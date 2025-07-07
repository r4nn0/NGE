#include "Entity.h"

Entity::Entity(std::string objName) : scale(glm::vec3(1.0f)),
                                      rotation(glm::vec3(0.0f)),
                                      position(glm::vec3(0.0f)),
                                      objectName(objName), model(nullptr){
    modelSet(objectName);
}
Entity::~Entity(){
    delete model;
}
void Entity::Render(){
    if(model==nullptr) return;
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), scale);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0),rotation.y, glm::vec3(0,1,0))*
                        glm::rotate(glm::mat4(1.0),rotation.x, glm::vec3(1,0,0))*
                        glm::rotate(glm::mat4(1.0),rotation.z, glm::vec3(0,0,1));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0), position);
    model->setModelMatrix(translationMatrix*rotationMatrix*scaleMatrix);
    ObjectsToRender.push_back(*model);
}
void Entity::modelSet(std::string objName){
    objectName=objName;
    //if(model!=nullptr) delete model;
    
    model = new Object3D();
    
    std::map<std::string, Object3D*>::iterator pos = ObjectsTotal.find(objName);
    if(pos==ObjectsTotal.end()){
        std::cout << "Object not found or not loaded to memory!" << std::endl;
        model->samplePlane2D();
        return;
    }
    *model = *(pos->second);
}