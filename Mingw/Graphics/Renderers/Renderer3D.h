#ifndef RENDERER3D_H
#define RENDERER3D_H

#include "../../ngestd.h"
#include "../TextureManager.h"

class Renderer3D{
public:
    Renderer3D();
    virtual ~Renderer3D();
    void LoadVerticesToGPU(Object3D&);
    void Render();
    /**
     * @brief A variable that counts drawcalls per frame
     * 
     */
protected:

private:
    GLuint m_timerQuery[2] = {};
    int    m_timerQueryIndex = 0;
    bool   m_timerQueryReady = false;

    
    unsigned int vboOffset, m_indexCount, m_instanceCount, m_morphPositionsOffset, m_jointOffset, m_morphWeightsOffset, m_nodeMatrixOffset, m_materialOffset, m_drawDataOffset;
    unsigned int m_VAO, m_VBO, m_IBO, m_Shader, m_ModelMatricesSSBO, m_MorphPositionsSSBO,m_JointMatricesSSBO, m_MorphWeightsSSBO, m_NodeMatricesSSBO, m_MaterialsSSBO, m_DrawDataSSBO;
    
    void *m_vboBase, *m_indexBase, *m_mmSSBOBase, *m_mpSSBOBase, *m_jmSSBOBase, *m_mwSSBOBase, *m_nmSSBOBase, *m_materialSSBOBase, *m_ddSSBOBase;
    GLsync m_fence;


    glm::mat4 m_lightSpaceMatrix;


    // Window size — needed to size the feedback FBO at 1/4 resolution
    glm::vec2 window_size;

    GLint m_uProjMatrix = -1;
    GLint m_uVwMatrix = -1;
    GLint m_uLightMode = -1;
    GLint m_uLightPos = -1;
    GLint m_uLightDir = -1;
    GLint m_uViewPos = -1;
    GLint m_uLightColor = -1;
    GLint m_uLightIntensity = -1;
    GLint m_uAmbientStrength = -1;
    
};

#endif // RENDERER2D_H