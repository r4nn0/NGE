#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "../ngestd.h"

#define MAX_SPRITE_COUNT 32000
#define VERTEX_SIZE sizeof(ngetype::vboData)
#define SPRITE_SIZE VERTEX_SIZE * 4
#define BUFFER_SIZE SPRITE_SIZE * MAX_SPRITE_COUNT
#define INDICES_SIZE MAX_SPRITE_COUNT * 6

class Renderer2D
{
public:
    Renderer2D();
    virtual ~Renderer2D();
    void Render();
    /**
     * @brief A variable that counts drawcalls per frame
     * 
     */
    unsigned int dcpf;
protected:

private:
    int m_maxTextures;
    ngetype::IBO* m_indexBuffer;
    GLsizei m_indexCount;
    unsigned int m_appSurface,m_VBO, m_Shader;
    ngetype::vboData* m_Buff;
};

#endif // RENDERER2D_H
