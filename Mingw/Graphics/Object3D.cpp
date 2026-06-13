#include "Object3D.h"
#include "TextureManager.h"
std::map<std::string, Object3D*> ObjectsTotal;
//TexutrePage TextureAtlas3D;
Object3D::Object3D(){
    
}
Object3D::Object3D(const char* file) : wasRendered(false), instanceID(-1), modelMatrix(glm::mat4(1.0f)){
    LoadModel(file);
}
bool Object3D::LoadModel(const char* file){
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    loader.LoadASCIIFromFile(&model, &err, &warn, file);
    if(err!=""){
        std::cout << "Error Loading: " << err << "\nWarning: "<< warn << std::endl;
        samplePlane2D();
        return false;
    }
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
    nodeGlobalMatrix.resize(nodeCount, glm::mat4(1.0f));

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
void Object3D::LoadMeshes() {
    for (int nodeIndex : scene->nodes)
    {
        LoadNode(nodeIndex);
    }
}
void Object3D::LoadNode(int nodeIndex){
    const tinygltf::Node& node = model.nodes[nodeIndex];
    
    if (node.mesh >= 0){
        const tinygltf::Mesh& mesh = model.meshes[node.mesh];
        
        for (const tinygltf::Primitive& primitive : mesh.primitives){
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

void Object3D::LoadVertices(const tinygltf::Primitive& primitive, Primitive& outPrimitive){


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
            vertex.texCoords = (texCoords.size() > i) ? glm::vec2(texCoords[i]) : glm::vec2(0.0f);
            vertex.texCoords = glm::vec2(glm::fract(vertex.texCoords.x), glm::fract(vertex.texCoords.y));
            auto packNormal = [](glm::vec3 n) -> unsigned {
                n = glm::normalize(n);
                unsigned x = (unsigned)((n.x * 0.5f + 0.5f) * 65535.0f) & 0xFFFF;
                unsigned y = (unsigned)((n.y * 0.5f + 0.5f) * 65535.0f) & 0xFFFF;
                return (x << 16) | y;
            };
            vertex.normal = (normals.size() > i) ? packNormal(normals[i]) : 0;
            auto packColor = [](glm::vec4 c) -> unsigned {
                return ((unsigned)(c.r * 255) & 0xFF) << 24 |
                    ((unsigned)(c.g * 255) & 0xFF) << 16 |
                    ((unsigned)(c.b * 255) & 0xFF) << 8  |
                    ((unsigned)(c.a * 255) & 0xFF);
            };
            vertex.color = (colors.size() > i) ? packColor(colors[i]) : 0xffffffff;
            auto packJoints = [](glm::uvec4 j) -> unsigned {
                return ((j.x & 0xFF) << 24) | ((j.y & 0xFF) << 16) |
                       ((j.z & 0xFF) <<  8) |  (j.w & 0xFF);
            };
            vertex.joints = (joints.size()>i) ? packJoints(joints[i]) : 0;
            auto packWeights = [](glm::vec4 w) -> unsigned {
                return ((unsigned)(w.x * 255) << 24) | ((unsigned)(w.y * 255) << 16) |
                       ((unsigned)(w.z * 255) <<  8) |  (unsigned)(w.w * 255);
            };
        vertex.weights = (weights.size()>i) ? packWeights(weights[i]) : 0;
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
    for (size_t targetIndex = 0; targetIndex < morphTargetCount; targetIndex++) {
        
        
        const auto& target = primitive.targets[targetIndex];
        if (target.find("POSITION")==target.end())
            continue;

        int accessorIndex = target.at("POSITION");

        const tinygltf::Accessor& accessor = model.accessors[accessorIndex];
        
        if (accessor.bufferView < 0 || accessor.bufferView >= model.bufferViews.size()){
            std::cerr << "Invalid bufferView index: " << accessor.bufferView << std::endl;
            return;
        }
        const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
        
        if (view.buffer < 0 ||
            view.buffer >= model.buffers.size())
        {
            std::cerr << "Invalid buffer index: "
                    << view.buffer << std::endl;

            std::cerr << "bufferViews.size() = "
                    << model.bufferViews.size() << std::endl;

            std::cerr << "buffers.size() = "
                    << model.buffers.size() << std::endl;

            return;
        }
        const tinygltf::Buffer& buffer = model.buffers[view.buffer];
        size_t vertexCount = accessor.count;
        outPrimitive.morphPositions[targetIndex].resize(vertexCount);
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
void Object3D::LoadTextures(){
    std::string basePath = "3DObjects/";
    TextureManager& texManager = TextureManager::getInstance();
    // First, load all images from the glTF model
    std::vector<int> textureIDs(model.images.size(), -1);
    for (size_t i = 0; i < model.images.size(); i++) {
        
        const auto& image = model.images[i];
        
        if (!image.image.empty()) {
            // Embedded image data (already decoded by tinygltf)
            std::string key = image.uri;
            std::cout << image.image.size() << std::endl;
            // tinygltf has already decoded the image, use it directly
            textureIDs[i] = texManager.loadTextureFromMemory(
                image.image.data(), 
                image.image.size(), 
                key, 
                false, // disable mipmaps
                image.width,
                image.height,
                image.component
            );
        } else if (image.bufferView >= 0) {
            // Image stored in a buffer view - tinygltf should have decoded it
            std::string key = image.uri;
            
            if (!image.image.empty()) {
                textureIDs[i] = texManager.loadTextureFromMemory(
                    image.image.data(), 
                    image.image.size(), 
                    key, 
                    false,
                    image.width,
                    image.height,
                    image.component
                );
            } else {
                std::cout << "Buffer view image is empty, skipping" << std::endl;
            }
        } else if (!image.uri.empty()) {
            // External image file
            std::string imagePath = basePath + image.uri;
            textureIDs[i] = texManager.loadTexture(imagePath.c_str(), false);
        } else {
            std::cout << "Image " << i << " has no data and no URI, skipping" << std::endl;
        }
    }
    
    materials.resize(model.materials.size());
    for (size_t i = 0; i < model.materials.size(); i++) {
        const auto& mat = model.materials[i];
        const auto& pbr = mat.pbrMetallicRoughness;
        Material& m = materials[i];
        
        // PBR factors
        m.baseColorFactor = glm::vec4(pbr.baseColorFactor[0], pbr.baseColorFactor[1],
                                      pbr.baseColorFactor[2], pbr.baseColorFactor[3]);
        m.metallicFactor  = (float)pbr.metallicFactor;
        m.roughnessFactor = (float)pbr.roughnessFactor;
        
        if (mat.extensions.find("KHR_materials_pbrSpecularGlossiness") != mat.extensions.end()) {
            // Access the extension data
            const auto& ext = mat.extensions.at("KHR_materials_pbrSpecularGlossiness");
            
            // Get diffuseTexture (equivalent to baseColorTexture)
            if (ext.Has("diffuseTexture")) {
                const auto& diffuseTexObj = ext.Get("diffuseTexture");
                if (diffuseTexObj.Has("index")) {
                    int texIdx = diffuseTexObj.Get("index").GetNumberAsInt();
                    if (texIdx >= 0 && texIdx < (int)model.textures.size()) {
                        int imgId = model.textures[texIdx].source;
                        if (imgId >= 0 && imgId < (int)textureIDs.size() && textureIDs[imgId] >= 0) {
                            m.albedoHandle = texManager.getTextureHandle(textureIDs[imgId]);
                            m.hasTexture |= 1;
                        }
                    }
                }
            }
        }
        else if (pbr.baseColorTexture.index >= 0 && pbr.baseColorTexture.index < (int)model.textures.size()) {
            int imgId = model.textures[pbr.baseColorTexture.index].source;
            if (imgId >= 0 && imgId < (int)textureIDs.size() && textureIDs[imgId] >= 0) {
                m.albedoHandle = texManager.getTextureHandle(textureIDs[imgId]);
                m.hasTexture |= 1;
            }
        }
        if (pbr.metallicRoughnessTexture.index >= 0 && pbr.metallicRoughnessTexture.index < (int)model.textures.size()) {
            int imgId = model.textures[pbr.metallicRoughnessTexture.index].source;
            if (imgId >= 0 && imgId < (int)textureIDs.size() && textureIDs[imgId] >= 0) {
                m.metallicHandle = texManager.getTextureHandle(textureIDs[imgId]);
                m.hasTexture |= 2;
            }
        }
        if (mat.normalTexture.index >= 0 && mat.normalTexture.index < (int)model.textures.size()) {
            int imgId = model.textures[mat.normalTexture.index].source;
            if (imgId >= 0 && imgId < (int)textureIDs.size() && textureIDs[imgId] >= 0) {
                m.normalHandle = texManager.getTextureHandle(textureIDs[imgId]);
                m.hasTexture |= 4;
            }
        }
        if (mat.emissiveTexture.index >= 0 && mat.emissiveTexture.index < (int)model.textures.size()) {
            int imgId = model.textures[mat.emissiveTexture.index].source;
            if (imgId >= 0 && imgId < (int)textureIDs.size() && textureIDs[imgId] >= 0) {
                m.emissiveHandle = texManager.getTextureHandle(textureIDs[imgId]);
                m.hasTexture |= 8;
            }
        }
        
        m.emissiveFactor  = glm::vec4(mat.emissiveFactor[0], mat.emissiveFactor[1], mat.emissiveFactor[2], 0.0f);
        m.normalScale         = (float)mat.normalTexture.scale;
        m.occlusionStrength   = (float)mat.occlusionTexture.strength;
        m.alphaMode           = (mat.alphaMode=="OPAQUE") ? 0: (mat.alphaMode=="MASK") ? 1 : 2;
        m.alphaCutoff         = (float)mat.alphaCutoff;
        m.doubleSided         = mat.doubleSided;
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
        
        UpdateNodeRecursive(rootNode, glm::mat4(1.0f));
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
    //botLeft.textureSlot = botRight.textureSlot=topRight.textureSlot=topLeft.textureSlot=0;
    botLeft.pos = glm::vec3(-5.f, 0,-5.f );
    botRight.pos = glm::vec3(5.f, 0,-5.f );
    topRight.pos = glm::vec3(5.f, 0, 5.f );
    topLeft.pos = glm::vec3(-5.f, 0, 5.f);
    botLeft.texCoords = glm::vec2(0,0);
    botRight.texCoords = glm::vec2(1,0);
    topRight.texCoords = glm::vec2(1,1);
    topLeft.texCoords = glm::vec2(0,1);
    botLeft.normal = botRight.normal = topLeft.normal = topRight.normal = 0;
    botLeft.color = botRight.color = topLeft.color = topRight.color = 0xffffffff;
    

    botLeft.joints = botRight.joints=topRight.joints=topLeft.joints=0;
    botLeft.weights = botRight.weights=topRight.weights=topLeft.weights=0;

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
    temp.materialIndex=0;
    primitives.push_back(temp);
    nodeLocalMatrix.push_back(glm::mat4(1.0f));
    nodeGlobalMatrix.push_back(glm::mat4(1.0f));
    
    Material mat;
    mat.hasTexture = 1;
    mat.baseColorFactor = glm::vec4(1.0f);
    TextureManager& texManager = TextureManager::getInstance();
    mat.albedoHandle = texManager.getTextureHandle(0);
    materials.push_back(mat);
}
void Object3D::switchTexture(int texID){
    TextureManager& texManager = TextureManager::getInstance();
    materials[0].albedoHandle = texManager.getTextureHandle(texID);
}