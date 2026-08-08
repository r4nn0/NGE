#ifndef RENDERER2D_H
#define RENDERER2D_H

#include <ngestd.h>


class Renderer2D {
public:
    Renderer2D();
    virtual ~Renderer2D();
    void Render(Object2D);
    void LoadShader(unsigned);
    void flush();
protected:

private:
    unsigned int m_VAO, m_VBO, m_IBO, m_SSBO, m_Shader;
    void* m_vboBase, *m_ssboBase;
    struct vboData{
        glm::vec3 pos;
        glm::vec2 texCoords;
        unsigned id;
        
    };
    struct ssboData{
        glm::vec3 pos;
        float rot;
        glm::vec4 color;
        glm::vec2 scale;
        int textureSlot;
        int pad;
    };
    GLsync m_fence;

    const unsigned MAX_SPRITES = 32000;
    const unsigned VBO_DATA_SIZE = sizeof(vboData);
    const unsigned VBO_SIZE = VBO_DATA_SIZE * 4 * MAX_SPRITES;
    const unsigned IBO_SIZE = MAX_SPRITES*6*sizeof(short);
    const unsigned SSBO_SIZE =  sizeof(ssboData) * MAX_SPRITES;
    std::vector<Object2D> objToRender;
};

#endif // RENDERER2D_H
