#ifndef PRIMITIVE_RENDERER_H
#define PRIMITIVE_RENDERER_H

#include <Graphics/Shape.h>
#include <ngestd.h>

class PrimitiveRenderer{
public:
    PrimitiveRenderer();
    virtual ~PrimitiveRenderer();
    void Render();
private:
    unsigned int m_VAO, m_VBO, m_Shader, m_IBO;
    void* m_vboBase, * m_iboBase;

    unsigned vboOffset, iboOffset;

    struct vboData{
        glm::vec3 pos;
        glm::vec3 lpos;
        float rot;
        glm::vec2 scale;
        glm::vec4 color;
    };

    static const int VBO_SIZE = sizeof(vboData) * 32000;
    static const int IBO_SIZE = sizeof(unsigned short) * 32000;
};

#endif