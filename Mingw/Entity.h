#ifndef ENTITY_HEADER
#define ENTITY_HEADER

#include "Graphics/Renderers/Renderer3D.h"

class Entity{
public:
    Entity(std::string="");
    ~Entity();
    void Update();
    void UpdateAnimation(float);
    void Render();
    void modelSet(std::string);
    Object3D& getModel();
    void setTexture(int texID);
    const std::string& getModelName()const { return objectName;} 
    glm::vec3 scale, rotation, position;
private:
    std::string objectName;
    Object3D* model;
};

#endif