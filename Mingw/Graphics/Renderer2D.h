#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "../ngestd.h"


class Renderer2D {
public:
    Renderer2D();
    virtual ~Renderer2D();
    void Render();
protected:

private:
    unsigned int m_appSurface, m_VBO, m_Shader, m_IBO;
    struct vboData{
        glm::vec3 pos;
        glm::vec4 color;
        glm::vec2 texCoords;
        float textureSlot;
    };
};

#endif // RENDERER2D_H
