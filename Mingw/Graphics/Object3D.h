#ifndef OBJECT3D_H
#define OBJECT3D_H
#include "../include/GLM/glm.hpp"
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <vector>
#include "tiny_gltf.h"
#include "Sprite.h"
class Object3D
{
public:
    struct Vertex3D
    {
        glm::vec3 pos;
        glm::vec4 color;
        glm::vec2 texCoords;
        glm::vec3 normal;
        float textureSlot;
    };
    Object3D(const char* objfile);
    Object3D();
    ~Object3D();
    glm::vec3 scale, rotation, position;

    void samplePlane2D();
    std::string name;
    const std::vector<Vertex3D> getVertices() const { return m_vertices; }
    const std::vector<unsigned int> getIndices() const { return m_indices; }
    const glm::mat4 getModelMatrix() const;
private:
    void LoadModel(const char*);
    std::vector<glm::vec4> readVecFloat(const tinygltf::Model& mdl, const tinygltf::Primitive& prim, const std::string& attr, int expectedComponents);
    unsigned getIndexFromAccessor(const tinygltf::Model& ,const tinygltf::Primitive&,const std::string&,const unsigned int) const;
    void readIndices(const tinygltf::Model& mdl, const tinygltf::Primitive& primitive);
    std::vector<Vertex3D> m_vertices;
    std::vector<unsigned int> m_indices;
};
//extern TexturePage TextureAtlas3D;
#endif // OBJECT3D_H