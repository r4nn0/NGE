#ifndef OBJECT3D_H
#define OBJECT3D_H
#include "../include/GLM/glm.hpp"
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <vector>

class Object3D
{
public:
    struct Vertex3D
    {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 texCoords;
        glm::vec4 color;
        float textureSlot;
    };
    Object3D(const char* objfile, const char* material=NULL);
    ~Object3D();
    glm::vec3 position;
    std::vector<Vertex3D> getVertices() const { return m_vertices; }
    std::vector<unsigned int> getIndices() const { return m_indices; }
private:
    std::vector<Vertex3D> m_vertices;
    std::vector<unsigned int> m_indices;
};

struct VertexKey{
    unsigned int v, vt, vn;
    bool operator==(const VertexKey& other) const
    {
        return v == other.v && vt == other.vt && vn == other.vn;
    }
};
namespace std {
    template<>
    struct hash<VertexKey> {
        size_t operator()(const VertexKey& key) const {
            return ((std::hash<unsigned int>()(key.v) ^
                        (std::hash<unsigned int>()(key.vt) << 1)) >> 1) ^
                    (std::hash<unsigned int>()(key.vn) << 1);
        }
    };
}
#endif // OBJECT3D_H