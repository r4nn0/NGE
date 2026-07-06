#ifndef POLYGON_H
#define POLYGON_H
#include "../Engine.h"

class Shape{
public:
    Shape();
    ~Shape();
    glm::vec3 position;
    glm::vec4 color;
    float rotation;
    glm::vec3 scale;
    bool outline;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned short> indices;
    static Shape Circle(glm::vec3, float radius, bool=false);
    static Shape Rectangle(glm::vec3, glm::vec3 = glm::vec3(1.0), bool=false);
    static Shape Triangle(glm::vec3, glm::vec3, glm::vec3, bool=false);
    static Shape Line(glm::vec3, glm::vec3);
};
#endif