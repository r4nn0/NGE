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
    struct vboData{
        glm::vec3 pos;
        glm::vec4 color;
        glm::vec2 texCoords;
        glm::vec3 vNormals;
        float textureSlot;
        float modelID;
        vboData& operator=(const Object3D::Vertex3D& vertex){
            pos = vertex.pos;
            color = vertex.color;
            texCoords = vertex.texCoords;
            vNormals = vertex.normal;
            textureSlot = vertex.textureSlot;
            return *this;
        }
    };
    unsigned int m_appSurface, m_VBO, m_IBO, m_Shader, m_SSBO;
    unsigned testTex, texBuffer;
    GLuint64 texHandle;
};

#endif // RENDERER2D_H