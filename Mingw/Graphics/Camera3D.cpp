#include "Camera3D.h"

Camera3D::Camera3D(glm::vec3 pos, glm::vec3 target, glm::vec3 up) : position(pos), target(target), up(up) {}
Camera3D::Camera3D() : position(0.0f, 0.0f, 10.0f), target(0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f) {}

void Camera3D::setPosition(glm::vec3 pos) {
    position = pos;
}
void Camera3D::setTarget(glm::vec3 target) {
    this->target = target;
}
void Camera3D::setUp(glm::vec3 up) {
    this->up = up;
}
glm::vec3 Camera3D::getPosition() const {
    return position;
}
glm::vec3 Camera3D::getTarget() const {
    return target;
}
glm::vec3 Camera3D::getUp() const {
    return up;
}

