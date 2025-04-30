#include "Object3D.h"

Object3D::Object3D(const char* objfile, const char* material) {
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
}
Object3D::~Object3D() {
    // Destructor implementation (if needed)
}