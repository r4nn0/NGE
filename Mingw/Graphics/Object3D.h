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
        glm::vec3 normal;
        glm::vec2 texCoords;
        glm::vec4 color;
        float textureSlot;
    };
    Object3D(const char* objfile);
    ~Object3D();
    
    glm::vec3 position;
    std::vector<Vertex3D> getVertices() const { return m_vertices; }
    std::vector<unsigned int> getIndices() const { return m_indices; }
    const glm::mat4& getModelMatrix(){return modelMatrix;}
    void setModelMatrix(const glm::mat4 mdl){modelMatrix=mdl;}
private:
    void LoadModel(const char*);
    std::vector<glm::vec4> readVecFloat(const tinygltf::Model& mdl, const tinygltf::Primitive& prim, const std::string& attr, int expectedComponents);
    unsigned getIndexFromAccessor(const tinygltf::Model& ,const tinygltf::Primitive&,const std::string&,int);
    void readIndices(const tinygltf::Model& mdl, const tinygltf::Primitive& primitive);
    std::vector<Vertex3D> m_vertices;
    std::vector<unsigned int> m_indices;
    glm::mat4 modelMatrix;
};
//extern TexturePage TextureAtlas3D;
#endif // OBJECT3D_H