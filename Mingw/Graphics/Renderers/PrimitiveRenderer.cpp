#include "PrimitiveRenderer.h"
PrimitiveRenderer::PrimitiveRenderer() : m_VAO((unsigned*)malloc(sizeof(unsigned)*2)), m_VBO((unsigned*)malloc(sizeof(unsigned)*2)), m_IBO((unsigned*)malloc(sizeof(unsigned)*2)){
    glCreateVertexArrays(2, m_VAO);

    glCreateBuffers(2, m_VBO);
    glNamedBufferStorage(m_VBO[0], VBO_SIZE, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    glNamedBufferStorage(m_VBO[1], VBO_SIZE, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    glCreateBuffers(2, m_IBO);
    glNamedBufferStorage(m_IBO[0], IBO_SIZE, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT );
    glNamedBufferStorage(m_IBO[1], IBO_SIZE, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT );

    glCreateBuffers(1, &m_SSBO);
    glNamedBufferStorage(m_SSBO, SSBO_SIZE, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    glVertexArrayVertexBuffer(m_VAO[0],0,m_VBO[0],0, sizeof(vboData));
    glVertexArrayElementBuffer(m_VAO[0], m_IBO[0]);

    glVertexArrayVertexBuffer(m_VAO[1],0,m_VBO[1],0, sizeof(vboData));
    glVertexArrayElementBuffer(m_VAO[1], m_IBO[1]);

    m_vboBase0 = glMapNamedBufferRange(m_VBO[0], 0, VBO_SIZE, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    m_iboBase0 = glMapNamedBufferRange(m_IBO[0], 0, IBO_SIZE, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    

    m_vboBase1 = glMapNamedBufferRange(m_VBO[1], 0, VBO_SIZE, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    m_iboBase1 = glMapNamedBufferRange(m_IBO[1], 0, IBO_SIZE, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    m_SSBOBase = glMapNamedBufferRange(m_SSBO, 0, SSBO_SIZE, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    for(int i=0;i<2;i++){
        glEnableVertexArrayAttrib(m_VAO[0],i);
        glVertexArrayAttribBinding(m_VAO[0],i,0);

        glEnableVertexArrayAttrib(m_VAO[1],i);
        glVertexArrayAttribBinding(m_VAO[1],i,0);
    }
    for(int v = 0;v<2;v++){
        glVertexArrayAttribFormat(m_VAO[v], 0, 3, GL_FLOAT, GL_FALSE, offsetof(vboData, pos));
        glVertexArrayAttribIFormat(m_VAO[v], 1, 1, GL_UNSIGNED_INT, offsetof(vboData, id));
    }
    m_Shader=CreateShader(LoadShaderFromFile("./Graphics/Shaders/primShader.vs").c_str(),
                          LoadShaderFromFile("./Graphics/Shaders/primShader.fs").c_str());
}
PrimitiveRenderer::~PrimitiveRenderer() {
    glDeleteBuffers(2,m_IBO);
    glDeleteBuffers(2,m_VBO);
    
    glDeleteProgram(m_Shader);
}
void PrimitiveRenderer::Render(Shape shape){
    shapesToRender.push_back(shape);
}
void PrimitiveRenderer::flush(){
    vboData* m_Buff[2];
    unsigned short* m_indexPtr[2];
    m_Buff[0] = (vboData*) m_vboBase0;
    m_Buff[1] = (vboData*) m_vboBase1;
    m_indexPtr[0] = (unsigned short*) m_iboBase0;
    m_indexPtr[1] = (unsigned short*) m_iboBase1;

    ssboData* m_ssboPtr = (ssboData*) m_SSBOBase;

    if(!m_ssboPtr){
        printf("Failed to bind buffer: %p", m_ssboPtr);
    }
    unsigned vboOffset[2], iboOffset[2];
    iboOffset[0] = iboOffset[1] = 0;
    vboOffset[0] = vboOffset[1] = 0;

    unsigned m_instanceCount = 0;
    for(Shape shape : shapesToRender){
        int outline = 0;
        if(shape.outline) outline=1;

        m_ssboPtr->pos = shape.position;
        m_ssboPtr->rot = shape.rotation;
        m_ssboPtr->scale = shape.scale;
        m_ssboPtr->color = shape.color;
        m_ssboPtr++;
        for(unsigned short idx : shape.indices){
            m_indexPtr[outline][iboOffset[outline]++] = idx + vboOffset[outline];
        }
        for(glm::vec3 vertex : shape.vertices){
            m_Buff[outline][vboOffset[outline]].pos = vertex;
            m_Buff[outline][vboOffset[outline]].id = m_instanceCount;
            vboOffset[outline]++;
        }
        m_instanceCount++;
    }
    shapesToRender.clear();
    

    
    
    glUseProgram(m_Shader);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SSBO);
    Camera cam = Engine::getInstance().camera;
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "vw_matrix"), 1, GL_FALSE, glm::value_ptr(cam.GetView2D()));
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "proj_matrix"), 1, GL_FALSE, glm::value_ptr(cam.GetProj2D()));
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(m_VAO[0]);
    glDrawElements(GL_TRIANGLES, iboOffset[0], GL_UNSIGNED_SHORT, NULL);
    
    glBindVertexArray(m_VAO[1]);
    glDrawElements(GL_LINES, iboOffset[1], GL_UNSIGNED_SHORT, NULL);

    glDisable(GL_BLEND);
    glBindVertexArray(0);
    glUseProgram(0);
}