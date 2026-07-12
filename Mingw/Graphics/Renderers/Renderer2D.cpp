#include "Renderer2D.h"

/**
 * @brief Initialize batch renderer to pass correct values to shaders
 * 
 */
Renderer2D::Renderer2D() : m_fence(nullptr){
    glCreateVertexArrays(1, &m_VAO);

    glCreateBuffers(1, &m_VBO);
    glNamedBufferStorage(m_VBO, VBO_SIZE, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    glVertexArrayVertexBuffer(m_VAO,0,m_VBO,0, VBO_DATA_SIZE);

    m_vboBase = glMapNamedBufferRange(m_VBO, 0, VBO_SIZE, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    unsigned short indices[IBO_SIZE];
    unsigned offset=0;
    for(unsigned i=0;i<IBO_SIZE;i+=6){
        indices[ i ]=offset;
        indices[i+1]=offset+1;
        indices[i+2]=offset+2;
        indices[i+3]=offset+2;
        indices[i+4]=offset+3;
        indices[i+5]=offset;
        offset+=4;
    }
    glCreateBuffers(1, &m_IBO);
    glNamedBufferStorage(m_IBO, IBO_SIZE, indices, 0);
    glVertexArrayElementBuffer(m_VAO, m_IBO);

    glCreateBuffers(1, &m_SSBO);
    glNamedBufferStorage(m_SSBO, SSBO_SIZE, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    m_ssboBase = glMapNamedBufferRange(m_SSBO, 0, SSBO_SIZE, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    for(int i=0;i<3;i++){
        glEnableVertexArrayAttrib(m_VAO,i);
        glVertexArrayAttribBinding(m_VAO,i,0);
    }

    glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(vboData, pos));
    glVertexArrayAttribFormat(m_VAO, 1, 2, GL_FLOAT, GL_FALSE, offsetof(vboData, texCoords));
    glVertexArrayAttribIFormat(m_VAO, 2, 1, GL_INT, offsetof(vboData, id));
    
    m_Shader=CreateShader(LoadShaderFromFile("./Graphics/Shaders/shader.vs").c_str(),
                          LoadShaderFromFile("./Graphics/Shaders/shader.fs").c_str());
}
/**
 * @brief Frees the buffers in memory
 * 
 */
Renderer2D::~Renderer2D() {
    glDeleteBuffers(1,&m_IBO);
    glDeleteBuffers(1,&m_VBO);
    glDeleteBuffers(1,&m_SSBO);
    glDeleteProgram(m_Shader);
}
void Renderer2D::Render(Object2D obj){
    objToRender.push_back(obj);
}
/**
 * @brief Start rendering objects added to the renderer
 * 
 */
void Renderer2D::flush(){
    GLsizei m_indexCount=0;
    vboData* m_Buff = (vboData*) m_vboBase;
    ssboData* m_ssboPtr = (ssboData*) m_ssboBase;
    if(!m_indexCount && m_fence){
        glClientWaitSync(m_fence, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED);
        glDeleteSync(m_fence);
        m_fence=nullptr;
    }
    unsigned m_instanceCount = 0;
    for(Object2D obj: objToRender){
        m_ssboPtr[m_instanceCount].pos = obj.position;
        m_ssboPtr[m_instanceCount].rot = obj.rotation; 
        m_ssboPtr[m_instanceCount].color = obj.color;
        m_ssboPtr[m_instanceCount].scale = obj.scale;
        m_ssboPtr[m_instanceCount].textureSlot = obj.textureSlot;

        glm::vec2& _origin = obj.origin;
        glm::vec2& _size = obj.getSpriteSize();
        std::vector<glm::vec2> uvs = obj.UVs[(int)obj.frame_index];
        std::vector<glm::vec3> _lpos;
        
        _lpos.push_back(glm::vec3(-_origin,0));
        _lpos.push_back(glm::vec3(_size.x-_origin.x,-_origin.y,0));
        _lpos.push_back(glm::vec3(_size - _origin, 0));
        _lpos.push_back(glm::vec3(-_origin.x,_size.y-_origin.y,0));

        for(int i=0;i<4;i++){
            m_Buff->pos = _lpos[i];
            m_Buff->texCoords = uvs[i];
            m_Buff->id = m_instanceCount;
            m_Buff++;
        }
        m_indexCount+=6;
        m_instanceCount++;
    }
    
    objToRender.clear();
    
    glUseProgram(m_Shader);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SSBO);
    Camera cam = Engine::getInstance().camera;
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "proj_matrix"),1,GL_FALSE, glm::value_ptr(cam.GetProj2D()));
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "vw_matrix"),1,GL_FALSE, glm::value_ptr(cam.GetView2D()));
    glUniform1i(glGetUniformLocation(m_Shader, "texture[0]"), MainTextureAtlas.GetTextureSlot());
    MainTextureAtlas.Bind();
    glBindVertexArray(m_VAO);
    
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, NULL);
    glDisable(GL_BLEND);
    glBindVertexArray(0);
    m_fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    MainTextureAtlas.Unbind();
    glUseProgram(0);
}
