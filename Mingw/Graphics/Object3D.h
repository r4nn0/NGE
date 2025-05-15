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
    Object3D();
    ~Object3D();
    glm::vec3 scale, rotation, position;
    void rotate(glm::vec3);
    void translate(glm::vec3);

    void samplePlane2D();

    std::vector<Vertex3D> getVertices() const { return m_vertices; }
    std::vector<unsigned int> getIndices() const { return m_indices; }
    glm::mat4 getModelMatrix() const{
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), scale);
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0),rotation.y, glm::vec3(0,1,0))*
                         glm::rotate(glm::mat4(1.0),rotation.x, glm::vec3(1,0,0))*
                         glm::rotate(glm::mat4(1.0),rotation.z, glm::vec3(0,0,1));
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0), position);
        return translationMatrix*rotationMatrix*scaleMatrix;
    }
private:
    void LoadModel(const char*);
    std::vector<glm::vec4> readVecFloat(const tinygltf::Model& mdl, const tinygltf::Primitive& prim, const std::string& attr, int expectedComponents);
    unsigned getIndexFromAccessor(const tinygltf::Model& ,const tinygltf::Primitive&,const std::string&,int);
    void readIndices(const tinygltf::Model& mdl, const tinygltf::Primitive& primitive);
    std::vector<Vertex3D> m_vertices;
    std::vector<unsigned int> m_indices;
};
//extern TexturePage TextureAtlas3D;
#endif // OBJECT3D_H