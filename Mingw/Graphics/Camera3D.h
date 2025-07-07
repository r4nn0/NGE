#ifndef CAMERA_3D_H
#define CAMERA_3D_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera3D{
public:
    Camera3D(glm::vec3, glm::vec3);
    Camera3D();
    void setPosition(glm::vec3);
    void setRotation(glm::vec3);
    void setOrbit(glm::vec3);

    glm::vec3 getPosition() const;
    glm::vec3 getRotation() const;
    glm::vec3 getOrbit() const;
    glm::mat4 getMatrix() const;
private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 orbit;
};

#endif // CAMERA_3D_H