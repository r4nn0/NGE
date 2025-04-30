#ifndef CAMERA_3D_H
#define CAMERA_3D_H
#include "../include/GLM/glm.hpp"
class Camera3D{
public:
    Camera3D(glm::vec3 pos, glm::vec3 target, glm::vec3 up);
    Camera3D();
    void setPosition(glm::vec3 pos);
    void setTarget(glm::vec3 target);
    void setUp(glm::vec3 up);
    glm::vec3 getPosition() const;
    glm::vec3 getTarget() const;
    glm::vec3 getUp() const;
private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
};

#endif // CAMERA_3D_H