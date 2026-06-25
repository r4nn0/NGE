#include "Renderer2D.h"
#define VERTEX_SIZE2D sizeof(vboData)
#define BUFFER_SIZE2D VERTEX_SIZE2D * 4 * 32000
#define INDICES_SIZE2D 32000*6
#define MODEL_MATRICES_SSBO_SIZE3D 2*MEGABYTE
/**
 * @brief Initialize batch renderer to pass correct values to shaders
 * 
 */
Renderer2D::Renderer2D() : m_fence(nullptr){
    glCreateVertexArrays(1, &m_VAO);

    glCreateBuffers(1, &m_VBO);
    glNamedBufferStorage(m_VBO, BUFFER_SIZE2D, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    glVertexArrayVertexBuffer(m_VAO,0,m_VBO,0, VERTEX_SIZE2D);

    m_vboBase = glMapNamedBufferRange(m_VBO, 0, BUFFER_SIZE2D, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    unsigned short indices[INDICES_SIZE2D];
    unsigned offset=0;
    for(unsigned i=0;i<INDICES_SIZE2D;i+=6){
        indices[ i ]=offset;
        indices[i+1]=offset+1;
        indices[i+2]=offset+2;
        indices[i+3]=offset+2;
        indices[i+4]=offset+3;
        indices[i+5]=offset;
        offset+=4;
    }
    glCreateBuffers(1, &m_IBO);
    glNamedBufferStorage(m_IBO, INDICES_SIZE2D*sizeof(short), indices, 0);
    glVertexArrayElementBuffer(m_VAO, m_IBO);


    for(int i=0;i<7;i++){
        glEnableVertexArrayAttrib(m_VAO,i);
        glVertexArrayAttribBinding(m_VAO,i,0);
    }

    glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(vboData, pos));
    glVertexArrayAttribFormat(m_VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(vboData, lpos));
    glVertexArrayAttribFormat(m_VAO, 2, 1, GL_FLOAT, GL_FALSE, offsetof(vboData, rot));
    glVertexArrayAttribFormat(m_VAO, 3, 2, GL_FLOAT, GL_FALSE, offsetof(vboData, scale));
    glVertexArrayAttribFormat(m_VAO, 4, 4, GL_FLOAT, GL_FALSE, offsetof(vboData, color));
    glVertexArrayAttribFormat(m_VAO, 5, 2, GL_FLOAT, GL_FALSE, offsetof(vboData, texCoords));
    glVertexArrayAttribIFormat(m_VAO, 6, 1, GL_INT, offsetof(vboData, textureSlot));
    
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
    
    glDeleteProgram(m_Shader);
}

/**
 * @brief Start rendering objects added to the renderer
 * 
 */
void Renderer2D::Render(){
    GLsizei m_indexCount=0;
    vboData* m_Buff = (vboData*) m_vboBase;

    if(!m_indexCount && m_fence){
        glClientWaitSync(m_fence, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED);
        glDeleteSync(m_fence);
        m_fence=nullptr;
    }

    for(Object2D obj: Objects2DtoRender){
        float textureSlot =obj.textureSlot;
        glm::vec3& _pos = obj.position;
        glm::vec2& _origin = obj.origin;
        float& _rot = obj.rotation;
        glm::vec2& _scale = obj.scale;
        glm::vec2& _size = obj.getSpriteSize();
        glm::vec4& _col = obj.color;
        std::vector<glm::vec2> uvs = obj.UVs[(int)obj.frame_index];
        std::vector<glm::vec3> _lpos;
        
        _lpos.push_back(glm::vec3(-_origin,0));
        _lpos.push_back(glm::vec3(_size.x-_origin.x,-_origin.y,0));
        _lpos.push_back(glm::vec3(_size - _origin, 0));
        _lpos.push_back(glm::vec3(-_origin.x,_size.y-_origin.y,0));

        for(int i=0;i<4;i++){
            m_Buff->pos = _pos;
            m_Buff->lpos = _lpos[i];
            m_Buff->rot = _rot;
            m_Buff->scale = _scale;
            m_Buff->color=_col;
            m_Buff->texCoords = uvs[i];
            m_Buff->textureSlot = textureSlot;
            m_Buff++;
        }
        m_indexCount+=6;
    }
    
    Objects2DtoRender.clear();
    
    glUseProgram(m_Shader);

    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "proj_matrix"),1,GL_FALSE,Engine::getInstance().getOrthoMatrix());
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "vw_matrix"),1,GL_FALSE,Engine::getInstance().getViewMatrix2D());
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
