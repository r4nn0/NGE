#ifndef NGETYPE_H
#define NGETYPE_H
#include <string.h>
#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"

namespace ngetype
{
    struct vboData{
        glm::vec3 vertex;
        glm::vec4 color;
        glm::vec2 texCoords;
        float texID;
    };
    class IBO{
    private:
        unsigned int id, count;
    public:
        IBO(unsigned short int *, int);
        void bind();
        void unbind();
        unsigned int getCount();
    };
};
#endif // NGETYPE_H
