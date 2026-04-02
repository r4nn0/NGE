#ifndef RENDERER3D_H
#define RENDERER3D_H

#include "../../ngestd.h"
#include "../VTexSystem.h"

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
    unsigned int m_appSurface, m_VBO, m_IBO, m_Shader, m_ModelMatricesSSBO, m_MorphPositionsSSBO,m_JointMatricesSSBO, m_MorphWeightsSSBO, m_NodeMatricesSSBO, m_MaterialsSSBO, m_DrawDataSSBO;
    unsigned testTex, texBuffer;
    GLuint64 texHandle;
    void *m_vboBase, *m_indexBase, *m_mmSSBOBase, *m_mpSSBOBase, *m_jmSSBOBase, *m_mwSSBOBase, *m_nmSSBOBase, *m_materialSSBOBase, *m_ddSSBOBase;
    GLsync m_fence;

    GLuint m_feedbackFBO     = 0;
    GLuint m_feedbackTex     [4];  // color attachment
    GLuint m_feedbackDepth   = 0;  // depth attachment
    unsigned int m_feedbackShader = 0;

    GLuint m_shadowFBO      = 0;
    GLuint m_shadowMap      = 0; // depth texture
    GLuint m_shadowShader   = 0;
    glm::mat4 m_lightSpaceMatrix;

    static constexpr int SHADOW_MAP_SIZE = 2048;

    // Window size — needed to size the feedback FBO at 1/4 resolution
    glm::vec2 window_size;

    GLint m_uProjMatrix = -1;
    GLint m_uVwMatrix = -1;
    GLint m_uLightPos = -1;
    GLint m_uLightDir = -1;
    GLint m_uViewPos = -1;
    GLint m_uLightColor = -1;
    GLint m_uLightInnerCutoff = -1;
    GLint m_uLightOuterCutoff = -1;
    GLint m_uPhysicalAlbedo = -1;
    GLint m_uPhysicalNormal = -1;
    GLint m_uPhysicalRoughness = -1;
    GLint m_uPhysicalEmissive = -1;
    GLint m_uPageTableAlbedo = -1;
    GLint m_uPageTableNormal = -1;
    GLint m_uPageTableRoughness = -1;
    GLint m_uPageTableEmissive = -1;
    GLint m_uPhysicalSize = -1;
    GLint m_uTileSize = -1;
    GLint m_uPageTableSize = -1;

    // Cached uniform locations — feedback shader
    GLint m_fbProjMatrix = -1;
    GLint m_fbVwMatrix = -1;
    GLint m_fbPageTableSize = -1;
};

#endif // RENDERER2D_H