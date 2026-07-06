#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
class Camera{
public:
    Camera(glm::vec2=glm::vec2(1.0));

    glm::vec3 position3D;
    glm::vec3 rotation3D;
    glm::vec3 orbit;
    float orbitDistance;
    float fov;
    glm::vec2 view_size;
    float znear3D;
    float zfar3D;

    glm::vec3 position2D;
    float rotation2D;

    glm::vec2 oldZoom2D;
    glm::vec2 zoom2D;
    float znear2D;
    float zfar2D;
    void ZoomAt(glm::vec2, glm::vec2);
    glm::mat4 GetView3D();
    glm::mat4 GetView2D();
    glm::mat4 GetProj3D();
    glm::mat4 GetProj2D();
    glm::quat GetRotationQuat() const;
    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;
private:

    glm::mat4 view3D;
    glm::mat4 view2D;
    glm::mat4 proj3D;
    glm::mat4 proj2D;
    unsigned dirty;

    enum DirtyBits{
        View3D = 1 << 0,
        View2D = 1 << 1,
        Proj3D = 1 << 2,
        Proj2D = 1 << 3
    };
    void MarkAllDirty(){
        dirty = 0xFFFFFFFF;
    }

    
};

#endif // CAMERA_H