#include "PrimitiveRenderer.h"
PrimitiveRenderer::PrimitiveRenderer(): vboOffset(0), iboOffset(0){
    glCreateVertexArrays(1, &m_VAO);

    glCreateBuffers(1, &m_VBO);
    glNamedBufferStorage(m_VBO, VBO_SIZE, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    
    glCreateBuffers(1, &m_IBO);
    glNamedBufferStorage(m_IBO, IBO_SIZE, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT );

    glVertexArrayVertexBuffer(m_VAO,0,m_VBO,0, sizeof(vboData));
    glVertexArrayElementBuffer(m_VAO, m_IBO);

    m_vboBase = glMapNamedBufferRange(m_VBO, 0, VBO_SIZE, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    m_iboBase = glMapNamedBufferRange(m_IBO, 0, IBO_SIZE, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    for(int i=0;i<5;i++){
        glEnableVertexArrayAttrib(m_VAO,i);
        glVertexArrayAttribBinding(m_VAO,i,0);
    }

    glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(vboData, pos));
    glVertexArrayAttribFormat(m_VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(vboData, lpos));
    glVertexArrayAttribFormat(m_VAO, 2, 1, GL_FLOAT, GL_FALSE, offsetof(vboData, rot));
    glVertexArrayAttribFormat(m_VAO, 3, 2, GL_FLOAT, GL_FALSE, offsetof(vboData, scale));
    glVertexArrayAttribFormat(m_VAO, 4, 4, GL_FLOAT, GL_FALSE, offsetof(vboData, color));
    
    m_Shader=CreateShader(LoadShaderFromFile("./Graphics/Shaders/primShader.vs").c_str(),
                          LoadShaderFromFile("./Graphics/Shaders/primShader.fs").c_str());
}
PrimitiveRenderer::~PrimitiveRenderer() {
    glDeleteBuffers(1,&m_IBO);
    glDeleteBuffers(1,&m_VBO);
    
    glDeleteProgram(m_Shader);
}
void PrimitiveRenderer::Render(){
    vboData* m_Buff = (vboData*) m_vboBase;
    unsigned short* m_indexPtr = (unsigned short*) m_iboBase;

    for(Shape shape : ShapesToRender){
        for(unsigned short idx : shape.indices){
            m_indexPtr[iboOffset++] = idx + vboOffset;
        }
        for(glm::vec3 vertex : shape.vertices){
            m_Buff[vboOffset].pos = shape.position;
            m_Buff[vboOffset].lpos = vertex;
            m_Buff[vboOffset].rot = shape.rotation;
            m_Buff[vboOffset].scale = shape.scale;
            m_Buff[vboOffset].color = shape.color;
            vboOffset++;
        }
    }
    ShapesToRender.clear();
    

    
    
    glUseProgram(m_Shader);

    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "proj_matrix"),1,GL_FALSE,Engine::getInstance().getOrthoMatrix());
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "vw_matrix"),1,GL_FALSE,Engine::getInstance().getViewMatrix2D());
    
    glBindVertexArray(m_VAO);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawElements(GL_LINES, iboOffset, GL_UNSIGNED_SHORT, NULL);
    glDisable(GL_BLEND);
    glBindVertexArray(0);
    glUseProgram(0);
    vboOffset = 0;
    iboOffset = 0;
}