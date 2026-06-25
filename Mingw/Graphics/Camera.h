#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera{
public:
    Camera(glm::vec3, glm::vec3);
    Camera();
    void setPosition(glm::vec3);
    void setRotation(glm::vec3);
    void setOrbit(glm::vec3);

    glm::vec3 getPosition() const;
    glm::vec3 getRotation() const;
    glm::vec3 getOrbit() const;
    glm::vec3 getForward() const;
    glm::mat4 getMatrix() const;
private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 orbit;
};

#endif // CAMERA_H