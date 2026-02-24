#include "Object3D.h"
std::map<std::string, Object3D*> ObjectsTotal;
//TexutrePage TextureAtlas3D;
Object3D::Object3D(){
    
}
Object3D::Object3D(const char* file) : modelMatrix(glm::mat4(1.0f)){
    LoadModel(file);
}
bool Object3D::LoadModel(const char* file){
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    loader.LoadBinaryFromFile(&model, &err, &warn, file);
    scene = &model.scenes[model.defaultScene];

    

    // Initialize nodes
    InitializeNodes();
    LoadTextures();
    // Load meshes
    LoadMeshes();

    // Load skins
    LoadSkins();

    // Initialize morph weights
    InitializeMorphWeights();
    
    

    CalculateAnimationDuration();
    return true;
}
void Object3D::InitializeNodes(){
    size_t nodeCount = model.nodes.size();

    nodeTranslation.resize(nodeCount);
    nodeRotation.resize(nodeCount);
    nodeScale.resize(nodeCount);

    nodeLocalMatrix.resize(nodeCount);
    nodeGlobalMatrix.resize(nodeCount);

    morphWeights.resize(nodeCount);

    for (size_t i = 0; i < nodeCount; i++)
    {
        const tinygltf::Node& node = model.nodes[i];

        // Translation
        if (node.translation.size() == 3)
        {
            nodeTranslation[i] = glm::vec3(
                node.translation[0],
                node.translation[1],
                node.translation[2]
            );
        }
        else
        {
            nodeTranslation[i] = glm::vec3(0.0f);
        }

        // Rotation
        if (node.rotation.size() == 4)
        {
            nodeRotation[i] = glm::quat(
                node.rotation[3],
                node.rotation[0],
                node.rotation[1],
                node.rotation[2]
            );
        }
        else
        {
            nodeRotation[i] = glm::quat(1,0,0,0);
        }

        // Scale
        if (node.scale.size() == 3)
        {
            nodeScale[i] = glm::vec3(
                node.scale[0],
                node.scale[1],
                node.scale[2]
            );
        }
        else
        {
            nodeScale[i] = glm::vec3(1.0f);
        }

        // Morph weights
        morphWeights[i].assign( node.weights.begin(), node.weights.end());
    }
}
void Object3D::LoadMeshes()
{
    for (int nodeIndex : scene->nodes)
    {
        LoadNode(nodeIndex);
    }
}
void Object3D::LoadNode(int nodeIndex){
    const tinygltf::Node& node = model.nodes[nodeIndex];

    if (node.mesh >= 0)
    {
        const tinygltf::Mesh& mesh = model.meshes[node.mesh];

        for (const tinygltf::Primitive& primitive : mesh.primitives)
        {
            LoadPrimitive(primitive, nodeIndex, node.skin);
        }
    }

    for (int child : node.children)
    {
        LoadNode(child);
    }
}
void Object3D::LoadPrimitive(const tinygltf::Primitive& primitive, int nodeIndex, int skinIndex){
    Primitive newPrimitive;

    newPrimitive.nodeIndex = nodeIndex;
    newPrimitive.skinIndex = skinIndex;
    newPrimitive.materialIndex = primitive.material;

    LoadVertices(primitive, newPrimitive);
    LoadIndices(primitive, newPrimitive);
    LoadMorphTargets(primitive, newPrimitive);

    primitives.push_back(std::move(newPrimitive));
}
std::vector<glm::vec4> Object3D::readVecFloat(const tinygltf::Primitive& primitive, const std::string& attr, int expectedComponents){
    std::vector<glm::vec4> vecOut;
    //for(auto attr : primitive.attributes)
        //std::cout << attr.first << " " << attr.second << std::endl;
    if(primitive.attributes.find(attr)==primitive.attributes.end()){
        //std::cerr << "Missing attribute: " << attr << std::endl;
        return vecOut;
    }
    const tinygltf::Accessor& posAccessor = model.accessors[primitive.attributes.at(attr)];
    const tinygltf::BufferView& posView = model.bufferViews[posAccessor.bufferView];
    const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
    const unsigned char* posDataPtr  = posBuffer.data.data() + posView.byteOffset + posAccessor.byteOffset;
    unsigned count = posAccessor.count;
    unsigned stride = posAccessor.ByteStride(posView);
    //std::cout << "bufferView.byteStride: " << posView.byteStride << std::endl;
    //std::cout << "computed stride: " << stride << std::endl;
    if(stride == 0)
        stride = expectedComponents*sizeof(float);
    for(unsigned i =0;i<count;i++){
        const float* elem = reinterpret_cast<const float*>(posDataPtr + i*stride);
        glm::vec4 vec(0.0f);
        for(int j=0;j<expectedComponents;j++)
            vec[j] = elem[j];
        vecOut.push_back(vec);
    }
    return vecOut;
}
void Object3D::LoadTextures(){
    
    hasTexture = false;
    std::vector<glm::vec2> imageOffsets(model.images.size());
    std::vector<glm::vec2> imageScaleFactors(model.images.size());
    for (size_t i = 0; i < model.images.size(); i++) {
        const auto& image = model.images[i];
        imageScaleFactors[i] = glm::vec2(image.width, image.height)/MainTextureAtlas.GetAtlasSize();
        imageOffsets[i] = MainTextureAtlas.TextureAdd(image.image, image.width, image.height);

        //std::cout << "Loaded image at offset: " << i << " (" << imageOffsets[i].x << "," << imageOffsets[i].y << ")" << std::endl;
        hasTexture=true;
    }
    
    if (model.materials.empty()) {
        return;
    }
    
    //materialTextureOffsets.resize(model.materials.size());
    materials.resize(model.materials.size());
    for (size_t i = 0; i < model.materials.size(); i++) {
        const tinygltf::Material& material = model.materials[i];
        const auto& pbr = material.pbrMetallicRoughness;
        Material& m = materials[i];
        m.baseColorFactor = glm::vec4(
        pbr.baseColorFactor[0], pbr.baseColorFactor[1],
        pbr.baseColorFactor[2], pbr.baseColorFactor[3]);
        m.metallicFactor  = (float)pbr.metallicFactor;
        m.roughnessFactor = (float)pbr.roughnessFactor;

        /*m.baseColorTexture          = pbr.baseColorTexture.index;
        m.metallicRoughnessTexture  = pbr.metallicRoughnessTexture.index;
        m.normalTexture             = material.normalTexture.index;
        m.occlusionTexture          = material.occlusionTexture.index;
        m.emissiveTexture           = material.emissiveTexture.index;*/

        m.emissiveFactor  = glm::vec4(material.emissiveFactor[0], material.emissiveFactor[1], material.emissiveFactor[2], 0.0f);
        m.normalScale         = (float)material.normalTexture.scale;
        m.occlusionStrength   = (float)material.occlusionTexture.strength;
        m.alphaMode           = (material.alphaMode=="OPAQUE") ? 0: (material.alphaMode=="MASK") ? 1 : 2;
        m.alphaCutoff         = (float)material.alphaCutoff;
        m.doubleSided         = material.doubleSided;


        if (pbr.baseColorTexture.index >= 0 /*&& (unsigned)pbr.baseColorTexture.index < model.textures.size()*/) {
            
            int imgIndex = model.textures[pbr.baseColorTexture.index].source;
            texOffset[imgIndex] = imageOffsets[imgIndex];
            texScaleFactor[imgIndex] = imageScaleFactors[imgIndex];

            /*printf("Material %zu -> imageIndex %d -> offset (%.3f, %.3f) scale (%.3f, %.3f)\n",
            i, imgIndex,
            imageOffsets[imgIndex].x, imageOffsets[imgIndex].y,
            imageScaleFactors[imgIndex].x, imageScaleFactors[imgIndex].y);*/
        }
        
        if(pbr.metallicRoughnessTexture.index >= 0){
            int imgIndex = model.textures[pbr.metallicRoughnessTexture.index].source;
            m.metallicRoughnessOffset = imageOffsets[imgIndex];
        }
        if(material.normalTexture.index >= 0){
            int imgIndex = model.textures[material.normalTexture.index].source;
            m.normalOffset = imageOffsets[imgIndex];
        }
        if(material.occlusionTexture.index >= 0){
            int imgIndex = model.textures[material.occlusionTexture.index].source;
            m.occlusionOffset = imageOffsets[imgIndex];
        }
        if(material.emissiveTexture.index >= 0){
            int imgIndex = model.textures[material.emissiveTexture.index].source;
            m.emissiveOffset = imageOffsets[imgIndex];
        }

    }
}
void Object3D::LoadVertices(const tinygltf::Primitive& primitive, Primitive& outPrimitive){
    /*const float* positions = nullptr;
    const float* normals = nullptr;
    const float* texcoords = nullptr;
    const float* weights = nullptr;
    const uint16_t* joints = nullptr;*/
    std::vector<glm::vec4> pos = readVecFloat(primitive, "POSITION", 3);
    std::vector<glm::vec4> normals = readVecFloat(primitive, "NORMAL", 3);
    std::vector<glm::vec4> texCoords = readVecFloat(primitive, "TEXCOORD_0", 2);
    std::vector<glm::vec4> colors = readVecFloat(primitive, "COLOR_0", 4);
    std::vector<glm::vec4> joints = readVecFloat(primitive, "JOINTS_0", 4);
    std::vector<glm::vec4> weights = readVecFloat(primitive, "WEIGHTS_0", 4);

    outPrimitive.vertices.resize(pos.size());
    for(unsigned i =0;i<pos.size();i++){
            Vertex3D vertex;
            vertex.pos = pos[i];
            //std::cout << pos[i].x << ",\t" << pos[i].y << ",\t" << pos[i].z << std::endl;
            vertex.normal = (normals.size() > i) ? normals[i] : glm::vec4(0.0f);
            vertex.color = (colors.size() > i) ? colors[i] : glm::vec4(1.0f);
            vertex.texCoords = (texCoords.size() > i) ? glm::vec2(texCoords[i]) : glm::vec2(0.0f);
            vertex.joints = (joints.size()>i) ? glm::uvec4(joints[i]) : glm::uvec4(0);
            vertex.weights = (weights.size()>i) ? weights[i] : glm::vec4(0);
            /*
            vertex.pos = pos[getIndexFromAccessor(mdl, primitive, "POSITION", i)];
            vertex.normal = (normals.size()>i) ? normals[getIndexFromAccessor(mdl, primitive, "NORMAL", i)] : glm::vec3(0.0f);
            vertex.color = (colors.size()>i) ? colors[getIndexFromAccessor(mdl, primitive, "COLOR_0", i)] : glm::vec4(1.0f);
            vertex.texCoords = (texCoords.size()>i) ? texCoords[getIndexFromAccessor(mdl, primitive, "TEXCOORD_0", i)] : glm::vec2(0.0);*/

            vertex.texCoords = glm::vec2(glm::fract(vertex.texCoords.x), glm::fract(vertex.texCoords.y));
            if(!texOffset.empty())
                vertex.texCoords = (vertex.texCoords*texScaleFactor[primitive.material])+texOffset[primitive.material];
                //printf("%f : %f\n", texOffset[primitive.material].x, texOffset[primitive.material].y);
            vertex.texCoords = glm::clamp(vertex.texCoords, glm::vec2(0.0f), glm::vec2(1.0f));
            //std::cout << vertex.texCoords.x << " " << vertex.texCoords.y << std::endl;
            if(hasTexture)
                vertex.textureSlot=MainTextureAtlas.GetTextureSlot();
            else
                vertex.textureSlot = -1;
            outPrimitive.vertices[i] = vertex;
    }
    
}
void Object3D::LoadIndices(const tinygltf::Primitive& primitive, Primitive& outPrimitive) {
    if (primitive.indices < 0)
        return;

    const tinygltf::Accessor& accessor =
        model.accessors[primitive.indices];

    const tinygltf::BufferView& view =
        model.bufferViews[accessor.bufferView];

    const tinygltf::Buffer& buffer =
        model.buffers[view.buffer];

    outPrimitive.indices.resize(accessor.count);

    
    switch (accessor.componentType) {
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
            const uint8_t* buf = reinterpret_cast<const uint8_t*>(buffer.data.data()+view.byteOffset+accessor.byteOffset);
            for (size_t i = 0; i < accessor.count; ++i)
                outPrimitive.indices[i]=static_cast<uint32_t>(buf[i]);
            break;
        }
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
            const uint16_t* buf = reinterpret_cast<const uint16_t*>(buffer.data.data()+view.byteOffset+accessor.byteOffset);
            for (size_t i = 0; i < accessor.count; ++i)
                outPrimitive.indices[i]=static_cast<uint32_t>(buf[i]);
            break;
        }
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
            const uint32_t* buf = reinterpret_cast<const uint32_t*>(buffer.data.data()+view.byteOffset+accessor.byteOffset);
            for (size_t i = 0; i < accessor.count; ++i)
                outPrimitive.indices[i]=static_cast<uint32_t>(buf[i]);
            break;
        }
        case TINYGLTF_COMPONENT_TYPE_FLOAT:{
            const float * buf = reinterpret_cast<const float*>(buffer.data.data()+view.byteOffset+accessor.byteOffset);
            for (size_t i = 0; i < accessor.count; ++i)
                outPrimitive.indices[i]=static_cast<uint32_t>(buf[i]);
            break;
        }
        default:
            std::cerr << "Unsupported index component type: " << accessor.componentType << std::endl;
            break;
    }
}
void Object3D::LoadMorphTargets(const tinygltf::Primitive& primitive, Primitive& outPrimitive){
    size_t morphTargetCount = primitive.targets.size();

    if (morphTargetCount == 0)
        return;

    outPrimitive.morphPositions.resize(morphTargetCount);

    for (size_t targetIndex = 0; targetIndex < morphTargetCount; targetIndex++)
    {
        const auto& target = primitive.targets[targetIndex];

        if (target.find("POSITION")==target.end())
            continue;

        int accessorIndex = target.at("POSITION");

        const tinygltf::Accessor& accessor =
            model.accessors[accessorIndex];

        const tinygltf::BufferView& view =
            model.bufferViews[accessor.bufferView];

        const tinygltf::Buffer& buffer =
            model.buffers[view.buffer];
        size_t vertexCount = accessor.count;
        outPrimitive.morphPositions[targetIndex].resize(vertexCount);
        /*const float* data =
            reinterpret_cast<const float*>(
                &buffer.data[accessor.byteOffset + view.byteOffset]);

        size_t vertexCount = accessor.count;

        outPrimitive.morphPositions[targetIndex].resize(vertexCount);

        for (size_t i = 0; i < vertexCount; i++)
        {
            outPrimitive.morphPositions[targetIndex][i] =
                glm::vec3(
                    data[i*3+0],
                    data[i*3+1],
                    data[i*3+2]);
        }*/
        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
            const float* data = reinterpret_cast<const float*>(
                &buffer.data[accessor.byteOffset + view.byteOffset]);
            
            for (size_t i = 0; i < vertexCount; i++) {
                outPrimitive.morphPositions[targetIndex][i] = glm::vec3(
                    data[i*3+0], data[i*3+1], data[i*3+2]);
            }
        }
        else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE) {
            const double* data = reinterpret_cast<const double*>(
                &buffer.data[accessor.byteOffset + view.byteOffset]);
            
            for (size_t i = 0; i < vertexCount; i++) {
                outPrimitive.morphPositions[targetIndex][i] = glm::vec3(
                    static_cast<float>(data[i*3+0]),
                    static_cast<float>(data[i*3+1]),
                    static_cast<float>(data[i*3+2]));
            }
        }
    }
}
void Object3D::LoadSkins(){
    skins.resize(model.skins.size());

    for (size_t skinIndex = 0; skinIndex < model.skins.size(); skinIndex++)
    {
        const tinygltf::Skin& gltfSkin = model.skins[skinIndex];

        Skin& skin = skins[skinIndex];

        skin.skinIndex = skinIndex;

        size_t jointCount = gltfSkin.joints.size();

        skin.inverseBindMatrices.resize(jointCount);

        skin.finalJointMatrices.resize(jointCount);

        if (gltfSkin.inverseBindMatrices < 0)
            continue;

        const tinygltf::Accessor& accessor =
            model.accessors[gltfSkin.inverseBindMatrices];

        const tinygltf::BufferView& view =
            model.bufferViews[accessor.bufferView];

        const tinygltf::Buffer& buffer =
            model.buffers[view.buffer];

        const glm::mat4* data = reinterpret_cast<const glm::mat4*>(&buffer.data[accessor.byteOffset + view.byteOffset]);

        for (size_t i = 0; i < jointCount; i++){
            //glm::mat4 mat;

            //memcpy(&mat, data + i*16, sizeof(glm::mat4));

            skin.inverseBindMatrices[i] = data[i];
        }
    }
}
void Object3D::InitializeMorphWeights(){
    for (size_t nodeIndex = 0; nodeIndex < model.nodes.size(); nodeIndex++)
    {
        const tinygltf::Node& node = model.nodes[nodeIndex];
        //morphWeights[nodeIndex].resize(node.weights.size);
        morphWeights[nodeIndex].assign( node.weights.begin(), node.weights.end());

        if (node.mesh >= 0)
        {
            const tinygltf::Mesh& mesh = model.meshes[node.mesh];

            if (!mesh.weights.empty())
            {
                morphWeights[nodeIndex].assign( mesh.weights.begin(), mesh.weights.end());
            }
        }
    }
}
void Object3D::CalculateAnimationDuration(){
    animationDurations.resize(model.animations.size());
    for (size_t animIndex = 0; animIndex < model.animations.size(); animIndex++)
    {
        const tinygltf::Animation& animation = model.animations[animIndex];
        float maxTime = 0.0f;
        
        for (const tinygltf::AnimationChannel& channel : animation.channels)
        {
            const tinygltf::AnimationSampler& sampler = animation.samplers[channel.sampler];
            const tinygltf::Accessor& inputAccessor = model.accessors[sampler.input];
            const tinygltf::BufferView& inputView = model.bufferViews[inputAccessor.bufferView];
            const tinygltf::Buffer& inputBuffer = model.buffers[inputView.buffer];
            const float* times = reinterpret_cast<const float*>(
                &inputBuffer.data[inputAccessor.byteOffset + inputView.byteOffset]);
            
            float lastTime = times[inputAccessor.count - 1];
            if (lastTime > maxTime)
                maxTime = lastTime;
        }
        
        animationDurations[animIndex] = maxTime;
    }
}
void Object3D::UpdateAnimation(float deltaTime){
    if (!playing) {
        //std::cout << "Animation not playing!" << std::endl;
        return;
    }

    if (model.animations.empty()){
        //std::cout << "No animations in model!" << std::endl;
        return;
    }
    
    float maxTime=animationDurations[currentAnimation];
    animationTime += deltaTime;
    if(animationTime>=maxTime) animationTime = fmod(animationTime, maxTime);
    
    const tinygltf::Animation& animation = model.animations[currentAnimation];
    
    for (const tinygltf::AnimationChannel& channel : animation.channels){
        const tinygltf::AnimationSampler& sampler =
            animation.samplers[channel.sampler];

        // Read input times
        const tinygltf::Accessor& inputAccessor =
            model.accessors[sampler.input];

        const tinygltf::BufferView& inputView =
            model.bufferViews[inputAccessor.bufferView];

        const tinygltf::Buffer& inputBuffer =
            model.buffers[inputView.buffer];

        const float* times =
            reinterpret_cast<const float*>(
                &inputBuffer.data[inputAccessor.byteOffset + inputView.byteOffset]);
        
        const tinygltf::Accessor& outputAccessor =
            model.accessors[sampler.output];

        const tinygltf::BufferView& outputView =
            model.bufferViews[outputAccessor.bufferView];

        const tinygltf::Buffer& outputBuffer =
            model.buffers[outputView.buffer];

        const float* values =
            reinterpret_cast<const float*>(
                &outputBuffer.data[outputAccessor.byteOffset + outputView.byteOffset]);

        int keyframe = 0;

        while ((unsigned)keyframe < inputAccessor.count - 1 && animationTime >= times[keyframe + 1])
            keyframe++;

        int nextKeyframe = std::min(keyframe + 1, (int)inputAccessor.count - 1);

        float t0 = times[keyframe];
        float t1 = times[nextKeyframe];

        float factor = (animationTime - t0) / (t1 - t0);

        int nodeIndex = channel.target_node;

        // Apply animation
        if (channel.target_path == "translation")
        {
            glm::vec3 v0 = glm::make_vec3(&values[keyframe*3]);
            glm::vec3 v1 = glm::make_vec3(&values[nextKeyframe*3]);

            nodeTranslation[nodeIndex] =
                glm::mix(v0, v1, factor);
        }
        else if (channel.target_path == "rotation")
        {
            glm::quat q0 = glm::make_quat(&values[keyframe*4]);
            glm::quat q1 = glm::make_quat(&values[nextKeyframe*4]);

            nodeRotation[nodeIndex] =
                glm::slerp(q0, q1, factor);
        }
        else if (channel.target_path == "scale")
        {
            glm::vec3 s0 = glm::make_vec3(&values[keyframe*3]);
            glm::vec3 s1 = glm::make_vec3(&values[nextKeyframe*3]);

            nodeScale[nodeIndex] =
                glm::mix(s0, s1, factor);
        }
        else if (channel.target_path == "weights")
        {
            size_t weightCount = outputAccessor.count / inputAccessor.count;

            morphWeights[nodeIndex].resize(weightCount);

            for (size_t i = 0; i < weightCount; i++)
            {
                float w0 = values[keyframe * weightCount + i];
                float w1 = values[nextKeyframe * weightCount + i];

                morphWeights[nodeIndex][i] =
                    glm::mix(w0, w1, factor);
            }
        }
    }
    
    UpdateNodeMatrices();

    UpdateSkinMatrices();
}
void Object3D::UpdateNodeMatrices(){
    for (size_t i = 0; i < model.nodes.size(); i++)
    {
        glm::mat4 T =
            glm::translate(glm::mat4(1), nodeTranslation[i]);

        glm::mat4 R =
            glm::mat4_cast(nodeRotation[i]);

        glm::mat4 S =
            glm::scale(glm::mat4(1), nodeScale[i]);

        nodeLocalMatrix[i] = T * R * S;
    }

    for (int rootNode : scene->nodes)
    {
        UpdateNodeRecursive(rootNode, glm::mat4(1));
    }
}
void Object3D::UpdateNodeRecursive(int nodeIndex, const glm::mat4& parent){
    nodeGlobalMatrix[nodeIndex] = parent * nodeLocalMatrix[nodeIndex];

    for (int child : model.nodes[nodeIndex].children)
    {
        UpdateNodeRecursive(child, nodeGlobalMatrix[nodeIndex]);
    }
}
void Object3D::UpdateSkinMatrices()
{
    for (size_t skinIndex = 0; skinIndex < skins.size(); skinIndex++)
    {
        const tinygltf::Skin& gltfSkin = model.skins[skinIndex];

        Skin& skin = skins[skinIndex];

        for (size_t jointIndex = 0; jointIndex < gltfSkin.joints.size(); jointIndex++)
        {
            int nodeIndex = gltfSkin.joints[jointIndex];

            skin.finalJointMatrices[jointIndex] =
                nodeGlobalMatrix[nodeIndex] *
                skin.inverseBindMatrices[jointIndex];
        }
    }
}
void Object3D::samplePlane2D(){
    Vertex3D botLeft, botRight, topRight, topLeft;
    botLeft.textureSlot = botRight.textureSlot=topRight.textureSlot=topLeft.textureSlot=0;
    botLeft.pos = glm::vec3(-5.f, 0,-5.f );
    botRight.pos = glm::vec3(5.f, 0,-5.f );
    topRight.pos = glm::vec3(5.f, 0, 5.f );
    topLeft.pos = glm::vec3(-5.f, 0, 5.f);
    botLeft.normal = botRight.normal = topLeft.normal = topRight.normal = glm::vec3(0,1.0f,0);
    botLeft.color = botRight.color = topLeft.color = topRight.color = glm::vec4(1,1,1,1);
    botLeft.texCoords = glm::vec2(0,0);
    botRight.texCoords = glm::vec2(1,0);
    topRight.texCoords = glm::vec2(1,1);
    topLeft.texCoords = glm::vec2(0,1);

    botLeft.joints = botRight.joints=topRight.joints=topLeft.joints=glm::uvec4(0);
    botLeft.weights = botRight.weights=topRight.weights=topLeft.weights=glm::vec4(0.0f);

    Primitive temp;
    temp.vertices.push_back(botLeft);
    temp.vertices.push_back(botRight);
    temp.vertices.push_back(topRight);
    temp.vertices.push_back(topLeft);
    temp.indices.push_back(0);
    temp.indices.push_back(1);
    temp.indices.push_back(2);

    temp.indices.push_back(0);
    temp.indices.push_back(2);
    temp.indices.push_back(3);
    temp.nodeIndex=0;
    primitives.push_back(temp);
    nodeLocalMatrix.push_back(glm::mat4(1.0f));
    nodeGlobalMatrix.push_back(glm::mat4(1.0f));
}