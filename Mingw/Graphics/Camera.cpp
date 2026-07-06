#include "Camera.h"
Camera::Camera(glm::vec2 window_size) : position3D(glm::vec3(0.0f)), rotation3D(glm::vec3(0.0f)), orbit(glm::vec3(0.0f)), orbitDistance(0.0f),
                       fov(60.0f), view_size(window_size), znear3D(0.1f), zfar3D(10000.0f),
                       position2D(glm::vec3(0.0f)), rotation2D(0.0f), oldZoom2D(glm::vec2(1.0f)),zoom2D(glm::vec2(1.0f)), znear2D(-128), zfar2D(127),
                       view3D(glm::mat4(1.0f)), view2D(glm::mat4(1.0f)), proj2D(glm::mat4(1.0f)), proj3D(glm::mat4(1.0f)),
                       dirty(0xFFFFFFFF){
}

void Camera::ZoomAt(glm::vec2 zoomFactor, glm::vec2 pos){
    zoom2D = glm::vec2(1.0f) + zoomFactor;
    if(oldZoom2D!=zoom2D){
        oldZoom2D = zoom2D;
        glm::vec2 viewNewSize = view_size/zoom2D;
        position2D = glm::vec3(pos-(viewNewSize/2.0f),0.0f)/2.0f;
    }
}
glm::mat4 Camera::GetView3D(){
    if (dirty & View3D) {

        glm::quat q = GetRotationQuat();
        glm::mat4 transform(1.0f);
        transform = glm::translate(transform, orbit);
        transform *= glm::mat4_cast(q);
        transform = glm::translate(transform, glm::vec3(0.0f,0.0f, orbitDistance));
        view3D = glm::inverse(transform);
        dirty &= ~View3D;
    }

    return view3D;
}
glm::mat4 Camera::GetView2D() {
    if (dirty & View2D){
        glm::mat4 view(1.0f);
        view = glm::translate(view, -position2D);
        view = glm::scale(view, glm::vec3(zoom2D, 1.0f));
        view = glm::rotate(view, -rotation2D, glm::vec3(0, 0, 1));
        view2D = view;
        dirty &= ~View2D;
    }
    return view2D;
}
glm::mat4 Camera::GetProj3D(){
    if (dirty & Proj3D){
        proj3D = glm::perspective(
            glm::radians(fov),
            view_size.x/view_size.y,
            znear3D,
            zfar3D
        );
        dirty &= ~Proj3D;
    }
    return proj3D;
}
glm::mat4 Camera::GetProj2D(){
    if (dirty & Proj2D){
        proj2D = glm::ortho(0.0f, view_size.x, view_size.y, 0.0f, -znear2D, -(zfar2D+1));
        dirty &= ~Proj2D;
    }

    return proj2D;
}
glm::quat Camera::GetRotationQuat() const{
    glm::quat qYaw   = glm::angleAxis(rotation3D.x, glm::vec3(0, 1, 0));
    glm::quat qPitch = glm::angleAxis(rotation3D.y, glm::vec3(1, 0, 0));
    glm::quat qRoll  = glm::angleAxis(rotation3D.z, glm::vec3(0, 0, 1));
    return qYaw * qPitch * qRoll;
}
glm::vec3 Camera::GetForward() const{
    return GetRotationQuat() * glm::vec3(0, 0, -1);
}
glm::vec3 Camera::GetRight() const{
    return GetRotationQuat() * glm::vec3(1, 0, 0);
}
glm::vec3 Camera::GetUp() const{
    return GetRotationQuat() * glm::vec3(0, 1, 0);
}