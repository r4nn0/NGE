#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "Sprite.h"

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
    void renderBegin();
    void addSprite(Sprite*);
    void Render();
    void renderEnd();
protected:

private:
    ngetype::IBO* m_indexBuffer;
    GLsizei m_indexCount;
    unsigned int m_appSurface,m_VBO;
    ngetype::vboData* m_Buff;
};

#endif // RENDERER2D_H
