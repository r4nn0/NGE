#ifndef POLYGON_H
#define POLYGON_H
#include "../Engine.h"

class Polygon{
public:
    Polygon();
    ~Polygon();
    std::vector<glm::vec3> vertices;
    std::vector<unsigned> indices;
};

#endif