#include "Shape.h"
std::vector<Shape> ShapesToRender;
Shape::Shape() : position(glm::vec3(0.0)), color(glm::vec4(1.0)), rotation(0.0f),scale(glm::vec2(1.0)), outline(false)
{}
Shape::~Shape(){

}
Shape Shape::Circle(float radius, bool outline){
    Shape circle;
    circle.outline = outline;
    circle.scale = glm::vec2(radius);
    int segments = 64;
    glm::vec3 vertex(0);
    if(!outline)
        circle.vertices.push_back(vertex);
    for(int i = 0; i < segments + outline; i++){
        float angle = glm::radians((float)i / (float)segments * 360.0f);
        float x = glm::cos(angle);
        float y = glm::sin(angle);
        circle.vertices.push_back(glm::vec3(x, y, 0));
    }
    unsigned short center = 0;
    for(int i = 0; i < segments; i++){
        if(outline) center = i;
        circle.indices.push_back(center);
        circle.indices.push_back(i+1);
        circle.indices.push_back((i+1)%segments + 1);
    }
    return circle;
}
Shape Shape::Rectangle(glm::vec2 size, bool outline){
    Shape rect;
    rect.outline = outline;
    float x1 = 0, y1 = 0, x2 = size.x, y2 = size.y;
    rect.vertices.push_back(glm::vec3(x1, y1, 0.0));
    rect.vertices.push_back(glm::vec3(x2, y1, 0.0));
    rect.vertices.push_back(glm::vec3(x2, y2, 0.0));
    rect.vertices.push_back(glm::vec3(x1, y2, 0.0));

    if(!outline){
        rect.indices.push_back(0);
        rect.indices.push_back(1);
        rect.indices.push_back(2);

        rect.indices.push_back(0);
        rect.indices.push_back(2);
        rect.indices.push_back(3);
        return rect;
    }
    rect.indices.push_back(0);
    rect.indices.push_back(1);

    rect.indices.push_back(1);
    rect.indices.push_back(2);

    rect.indices.push_back(2);
    rect.indices.push_back(3);

    rect.indices.push_back(3);
    rect.indices.push_back(0);
    return rect;
}
Shape Shape::Triangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, bool outline){
    Shape triangle;
    triangle.outline = outline;
    triangle.vertices.push_back(glm::vec3(v1, 0.0));
    triangle.vertices.push_back(glm::vec3(v2, 0.0));
    triangle.vertices.push_back(glm::vec3(v3, 0.0));

    if(!outline){
        triangle.indices.push_back(0);
        triangle.indices.push_back(1);
        triangle.indices.push_back(2);
        return triangle;
    }
    triangle.indices.push_back(0);
    triangle.indices.push_back(1);

    triangle.indices.push_back(1);
    triangle.indices.push_back(2);

    triangle.indices.push_back(2);
    triangle.indices.push_back(0);
    return triangle;
}