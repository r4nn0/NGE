#ifndef RENDERER3D_H
#define RENDERER3D_H

#include "../ngestd.h"

class Renderer3D{
public:
    Renderer3D();
    virtual ~Renderer3D();
    void Render();
    /**
     * @brief A variable that counts drawcalls per frame
     * 
     */
protected:

private:
    
    unsigned int m_appSurface, m_VBO, m_IBO, m_Shader, m_ModelMatricesSSBO, m_MorphPositionsSSBO,m_JointMatricesSSBO, m_MorphWeightsSSBO, m_NodeMatricesSSBO, m_MaterialsSSBO;
    unsigned testTex, texBuffer;
    GLuint64 texHandle;
    void *m_vboBase, *m_indexBase, *m_mmSSBOBase, *m_mpSSBOBase, *m_jmSSBOBase, *m_mwSSBOBase, *m_nmSSBOBase, *m_materialSSBOBase;
    GLsync m_fence;
};

#endif // RENDERER2D_H