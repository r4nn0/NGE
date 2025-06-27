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
    unsigned int m_appSurface, m_VBO, m_IBO, m_Shader, m_SSBO;
    unsigned testTex, texBuffer;
    GLuint64 texHandle;
};

#endif // RENDERER2D_H