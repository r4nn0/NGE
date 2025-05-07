#include "Object3D.h"
//TexutrePage TextureAtlas3D;
Object3D::Object3D(const char* file) {
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
            glm::vec2 texCoordsOffset(0.0f);
            if(primitive.material>=0){
                const tinygltf::Material& material = mdl.materials[primitive.material];
                const auto &pbr = material.pbrMetallicRoughness;
                if(material.pbrMetallicRoughness.baseColorTexture.index>=0){
                    const auto& texture = mdl.textures[material.pbrMetallicRoughness.baseColorTexture.index];
                    const auto& image = mdl.images[texture.source];

                    texCoordsOffset = MainTextureAtlas.TextureAdd((unsigned char*)image.image.data(), image.width, image.height);
                }
            }

            if(primitive.indices>=0)
                readIndices(mdl, primitive);
            for(unsigned i =0;i<pos.size();i++){
                Vertex3D vertex;
                vertex.pos = pos[i];
                vertex.normal = (normals.size()>i) ? normals[i] : glm::vec3(0.0f);
                //vertex.color = glm::vec4(1.0f);
                vertex.color = (colors.size()>i) ? colors[i] : glm::vec4(1.0f);
                vertex.texCoords = (texCoords.size()>i) ? texCoords[i] : glm::vec2(0.0);
                vertex.texCoords += texCoordsOffset;
                vertex.texCoords = glm::clamp(vertex.texCoords, glm::vec2(0.0f), glm::vec2(1.0f));
                std::cout << "X: " << vertex.texCoords.x << " Y: " << vertex.texCoords.y << std::endl;
                vertex.textureSlot=0;
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
        default:
            std::cerr << "Unsupported index component type: " << posAccessor.componentType << std::endl;
            break;
    }
}
std::vector<glm::vec4> Object3D::readVecFloat(const tinygltf::Model& mdl, const tinygltf::Primitive& primitive, const std::string& attr, int expectedComponents){
    std::vector<glm::vec4> vecOut;
    if(primitive.attributes.find(attr)==primitive.attributes.end())
        return vecOut;
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