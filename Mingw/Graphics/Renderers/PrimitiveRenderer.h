#ifndef PRIMITIVE_RENDERER_H
#define PRIMITIVE_RENDERER_H

#include <Graphics/Shape.h>
#include <ngestd.h>

class PrimitiveRenderer{
public:
    PrimitiveRenderer();
    virtual ~PrimitiveRenderer();
    void Render(Shape);
    void flush();
private:
    unsigned int m_Shader;
    unsigned *m_VAO, *m_VBO, *m_IBO, m_SSBO;
    void* m_vboBase0, * m_iboBase0, * m_vboBase1, * m_iboBase1, *m_SSBOBase;

    struct vboData{
        glm::vec3 pos;
        unsigned id;
    };
    struct ssboData{
        glm::vec3 pos;
        float rot;
        glm::vec3 scale;
        float pad;
        glm::vec4 color;
    };

    static const int VBO_SIZE = sizeof(vboData) * 32000;
    static const int IBO_SIZE = sizeof(unsigned short) * 32000;
    static const int SSBO_SIZE = sizeof(ssboData) * 32000;

    std::vector<Shape> shapesToRender;
};

#endif