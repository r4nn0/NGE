#include "Camera3D.h"

Camera3D::Camera3D(glm::vec3 pos, glm::vec3 rot) : position(pos), rotation(rot), orbit(pos) {}
Camera3D::Camera3D() : position(0), rotation(0), orbit(0) {}

void Camera3D::setPosition(glm::vec3 pos) {
    position = pos;
}
void Camera3D::setRotation(glm::vec3 rot) {
    rotation = rot;
}
void Camera3D::setOrbit(glm::vec3 orb){
    orbit = orb;
}
glm::vec3 Camera3D::getPosition() const {
    return position;
}
glm::vec3 Camera3D::getRotation() const {
    return rotation;
}
glm::vec3 Camera3D::getOrbit() const {
    return orbit;
}
glm::vec3 Camera3D::getForward() const {
    // Build the rotation matrix from your Euler angles
    glm::mat4 _rotation = glm::mat4(1.0f);
    _rotation = glm::rotate(_rotation, rotation.x, glm::vec3(0, 1, 0)); // Yaw
    _rotation = glm::rotate(_rotation, rotation.y, glm::vec3(1, 0, 0)); // Pitch
    _rotation = glm::rotate(_rotation, rotation.z, glm::vec3(0, 0, 1)); // Roll
    
    // Forward is -Z in camera space
    glm::vec3 forward = glm::normalize(glm::vec3(_rotation * glm::vec4(0, 0, -1, 0)));
    return forward;
}
glm::mat4 Camera3D::getMatrix() const {
    glm::mat4 cT = glm::mat4(1.0f);
    float relPos = glm::length(position-orbit);
    cT = glm::translate(cT, orbit);
    cT = glm::rotate(cT,rotation.x, glm::vec3(0,1,0));
    cT = glm::rotate(cT,rotation.y, glm::vec3(1,0,0));
    cT = glm::rotate(cT,rotation.z, glm::vec3(0,0,1));
    cT = glm::translate(cT, glm::vec3(0,0,relPos));
    return glm::inverse(cT);
}
