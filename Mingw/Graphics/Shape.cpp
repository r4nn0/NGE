#include "Shape.h"
Shape::Shape() : position(glm::vec3(0.0)), color(glm::vec4(1.0)), rotation(0.0f),scale(glm::vec3(1.0)), outline(false)
{}
Shape::~Shape(){

}
Shape Shape::Circle(glm::vec3 pos, float radius, bool outline){
    Shape circle;
    circle.outline = outline;
    circle.scale = glm::vec3(radius);
    circle.position = pos;
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
Shape Shape::Rectangle(glm::vec3 pos, glm::vec3 size, bool outline){
    Shape rect;
    rect.outline = outline;
    rect.position = pos;
    rect.scale = size;
    
    rect.vertices.push_back(glm::vec3(0.0));
    rect.vertices.push_back(glm::vec3(1, 0.0, 0.0));
    rect.vertices.push_back(glm::vec3(1.0, 1.0, 0.0));
    rect.vertices.push_back(glm::vec3(0.0, 1.0, 0.0));

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
Shape Shape::Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, bool outline){
    Shape triangle;
    triangle.outline = outline;
    triangle.position = v1;
    triangle.vertices.push_back(glm::vec3(0,0,0));
    triangle.vertices.push_back(v2 - v1);
    triangle.vertices.push_back(v3 - v1);

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
Shape Shape::Line(glm::vec3 v1, glm::vec3 v2){
    Shape line;
    line.outline = true;
    line.position = v1;
    line.vertices.push_back(glm::vec3(0,0,0));
    line.vertices.push_back(v2-v1);

    line.indices.push_back(0);
    line.indices.push_back(1);
    return line;

}