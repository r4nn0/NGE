#include "Object3D.h"
std::map<std::string, Object3D*> ObjectsTotal;
//TexutrePage TextureAtlas3D;
Object3D::Object3D(const char* file) : modelMatrix(glm::mat4(1.0f)){
    LoadModel(file);
    /*
    std::ifstream file(objfile);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << objfile << std::endl;
        return;
    }
    std::string line;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::unordered_map<VertexKey, unsigned int> vertexMap;
    while (std::getline(file, line)) {
        if (line.substr(0, 2) == "v ") {
            std::istringstream iss(line.substr(2));
            glm::vec3 position;
            iss >> position.x >> position.y >> position.z;
            positions.push_back(position);
        } else if (line.substr(0, 3) == "vn ") {
            std::istringstream iss(line.substr(3));
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } else if (line.substr(0, 3) == "vt ") {
            std::istringstream iss(line.substr(3));
            glm::vec2 texCoord;
            iss >> texCoord.x >> texCoord.y;
            texCoords.push_back(texCoord);
        } else if (line.substr(0, 2) == "f ") {
            std::istringstream iss(line.substr(2));
            while(!iss.eof()){
                char slash;
                unsigned int v,vt,vn;
                iss >> v >> slash >> vt >> slash >> vn;
                v--;
                vt--;
                vn--;
                VertexKey key = {v, vt, vn};
                if(vertexMap.find(key) == vertexMap.end()){
                    vertexMap[key] = m_vertices.size();
                    Vertex3D vertex;
                    vertex.pos = positions[v];
                    vertex.normal = normals[vn];
                    vertex.texCoords = texCoords[vt];
                    vertex.color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
                    vertex.textureSlot = 0;
                    m_vertices.push_back(vertex);
                }
                m_indices.push_back(vertexMap[key]);
            }
        }
    }
    //std::cout << "Loaded " << m_vertices.size() << " vertices and " << m_indices.size() << " indices from " << objfile << std::endl;   
    */ 
}
Object3D::Object3D() : modelMatrix(glm::mat4(1.0f)){
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
    m_indices.push_back(0);
    m_indices.push_back(1);
    m_indices.push_back(2);
    
    m_indices.push_back(0);
    m_indices.push_back(2);
    m_indices.push_back(3);
    
    m_vertices.push_back(botLeft);
    m_vertices.push_back(botRight);
    m_vertices.push_back(topRight);
    m_vertices.push_back(topLeft);
}
Object3D::~Object3D() {
    // Destructor implementation (if needed)
}
void Object3D::LoadModel(const char* path){
    tinygltf::Model mdl;
    tinygltf::TinyGLTF ldr;
    std::string err, warn;
    ldr.LoadBinaryFromFile(&mdl, &err, &warn, path);

    for(const auto& mesh: mdl.meshes){
        for(const auto& primitive : mesh.primitives){
            
            std::vector<glm::vec4> pos = readVecFloat(mdl,primitive, "POSITION", 3);
            std::vector<glm::vec4> normals = readVecFloat(mdl,primitive, "NORMAL", 3);
            std::vector<glm::vec4> texCoords = readVecFloat(mdl, primitive, "TEXCOORD_0", 2);
            std::vector<glm::vec4> colors = readVecFloat(mdl,primitive, "COLOR_0", 4);
            glm::vec2 texScaleFactor(1.0f);
            glm::vec2 texOffset(0.0f);
            if(primitive.material>=0 && primitive.material < mdl.materials.size()){
                const tinygltf::Material& material = mdl.materials[primitive.material];
                const auto &pbr = material.pbrMetallicRoughness;
                if(pbr.baseColorTexture.index>=0 && pbr.baseColorTexture.index<mdl.textures.size()){
                    const auto& texture = mdl.textures[pbr.baseColorTexture.index];
                    if(texture.source>=0 && texture.source<mdl.images.size()){
                        const auto& image = mdl.images[texture.source];
                        
                        texScaleFactor = glm::vec2(image.width, image.height)/MainTextureAtlas.GetAtlasSize();
                        //std::cout << texScaleFactor.x << " " << texScaleFactor.y << std::endl;
                        texOffset = MainTextureAtlas.TextureAdd(image.image, image.width, image.height);
                    }
                }
            }
            
            if(primitive.indices>=0 && primitive.indices<mdl.accessors.size())
                readIndices(mdl, primitive);
            for(unsigned i =0;i<pos.size();i++){
                
                Vertex3D vertex;
                vertex.pos = pos[i];
                vertex.normal = (normals.size() > i) ? normals[i] : glm::vec4(0.0f);
                vertex.color = (colors.size() > i) ? colors[i] : glm::vec4(1.0f);
                vertex.texCoords = (texCoords.size() > i) ? glm::vec2(texCoords[i]) : glm::vec2(0.0f);
                
                /*vertex.pos = pos[getIndexFromAccessor(mdl, primitive, "POSITION", i)];
                vertex.normal = (normals.size()>i) ? normals[getIndexFromAccessor(mdl, primitive, "NORMAL", i)] : glm::vec3(0.0f);
                vertex.color = (colors.size()>i) ? colors[getIndexFromAccessor(mdl, primitive, "COLOR_0", i)] : glm::vec4(1.0f);
                vertex.texCoords = (texCoords.size()>i) ? texCoords[getIndexFromAccessor(mdl, primitive, "TEXCOORD_0", i)] : glm::vec2(0.0);*/

                vertex.texCoords = (vertex.texCoords*texScaleFactor)+texOffset;
                vertex.texCoords = glm::clamp(vertex.texCoords, glm::vec2(0.0f), glm::vec2(1.0f));
                //std::cout << vertex.texCoords.x << " " << vertex.texCoords.y << std::endl;

                vertex.textureSlot=MainTextureAtlas.GetTextureSlot();
                m_vertices.push_back(vertex);
            }
        }
    }
}
void Object3D::readIndices(const tinygltf::Model& mdl, const tinygltf::Primitive& primitive){
    const tinygltf::Accessor& posAccessor = mdl.accessors[primitive.indices];
    const tinygltf::BufferView& posView = mdl.bufferViews[posAccessor.bufferView];
    const tinygltf::Buffer& posBuffer = mdl.buffers[posView.buffer];
    const unsigned char* posDataPtr  = posBuffer.data.data() + posView.byteOffset + posAccessor.byteOffset;
    unsigned count = posAccessor.count;

    switch (posAccessor.componentType) {
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
            const uint8_t* buf = reinterpret_cast<const uint8_t*>(posDataPtr);
            for (size_t i = 0; i < count; ++i)
                m_indices.push_back(static_cast<uint32_t>(buf[i]));
            break;
        }
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
            const uint16_t* buf = reinterpret_cast<const uint16_t*>(posDataPtr);
            for (size_t i = 0; i < count; ++i)
                m_indices.push_back(static_cast<uint32_t>(buf[i]));
            break;
        }
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
            const uint32_t* buf = reinterpret_cast<const uint32_t*>(posDataPtr);
            for (size_t i = 0; i < count; ++i)
                m_indices.push_back(buf[i]);
            break;
        }
        case TINYGLTF_COMPONENT_TYPE_FLOAT:{
            const float * buf = reinterpret_cast<const float*>(posDataPtr);
            for (size_t i = 0; i < count; ++i)
                m_indices.push_back(static_cast<uint32_t>(buf[i]));
            break;
        }
        default:
            std::cerr << "Unsupported index component type: " << posAccessor.componentType << std::endl;
            break;
    }
}
std::vector<glm::vec4> Object3D::readVecFloat(const tinygltf::Model& mdl, const tinygltf::Primitive& primitive, const std::string& attr, int expectedComponents){
    std::vector<glm::vec4> vecOut;
    if(primitive.attributes.find(attr)==primitive.attributes.end()){
        std::cerr << "Missing attribute: " << attr << std::endl;
        return vecOut;
    }
    const tinygltf::Accessor& posAccessor = mdl.accessors[primitive.attributes.at(attr)];
    const tinygltf::BufferView& posView = mdl.bufferViews[posAccessor.bufferView];
    const tinygltf::Buffer& posBuffer = mdl.buffers[posView.buffer];
    const unsigned char* posDataPtr  = posBuffer.data.data() + posView.byteOffset + posAccessor.byteOffset;
    unsigned count = posAccessor.count;
    unsigned stride = posAccessor.ByteStride(posView);
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
/*
unsigned Object3D::getIndexFromAccessor(const tinygltf::Model& model,
                         const tinygltf::Primitive& primitive,
                         const std::string& attributeName,
                         const unsigned int vertexIndex) const
{
    auto attrIt = primitive.attributes.find(attributeName);
    if (attrIt == primitive.attributes.end()) {
        return -1; // Attribute not found
    }

    const tinygltf::Accessor& accessor = model.accessors[attrIt->second];

    // If the accessor has a bufferView, fetch the data index
    if (vertexIndex >= accessor.count) {
        return -1; // Out of bounds
    }

    // Accessors can have byte strides and offsets.
    // But if you're just accessing raw index value for each vertex:
    return accessor.byteOffset / tinygltf::GetComponentSizeInBytes(accessor.componentType) + vertexIndex;
}*/
//const glm::mat4 Object3D::getModelMatrix() const {
//    return modelMatrix;
    /*
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), scale);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0),rotation.y, glm::vec3(0,1,0))*
                        glm::rotate(glm::mat4(1.0),rotation.x, glm::vec3(1,0,0))*
                        glm::rotate(glm::mat4(1.0),rotation.z, glm::vec3(0,0,1));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0), position);
    return translationMatrix*rotationMatrix*scaleMatrix;*/
//}
void Object3D::setModelMatrix(glm::mat4 mdl_matrix){
    modelMatrix = mdl_matrix;
}