#ifndef RENDERER3D_H
#define RENDERER3D_H

#include "../ngestd.h"

class Renderer3D
{
public:
    Renderer3D();
    virtual ~Renderer3D();
    void Render();
    /**
     * @brief A variable that counts drawcalls per frame
     * 
     */
    unsigned int dcpf;
protected:

private:
    //int m_maxTextures;
    ngetype::IBO* m_indexBuffer;
    //GLsizei m_indexCount;
    unsigned int m_appSurface, m_VBO, m_Shader, m_SSBO;
    //ngetype::vbo3DData* m_Buff;
};

#endif // RENDERER2D_H