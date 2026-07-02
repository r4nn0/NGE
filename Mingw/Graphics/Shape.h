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
    glm::vec2 scale;
    bool outline;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned short> indices;
    static Shape Circle(float radius, bool=false);
    static Shape Rectangle(glm::vec2 = glm::vec2(1.0), bool=false);
    static Shape Triangle(glm::vec2, glm::vec2, glm::vec2, bool=false);
};
extern std::vector<Shape> ShapesToRender;
#endif