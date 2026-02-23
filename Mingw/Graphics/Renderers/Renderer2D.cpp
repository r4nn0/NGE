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
    glCreateVertexArrays(1, &m_appSurface);

    glCreateBuffers(1, &m_VBO);
    glNamedBufferStorage(m_VBO, BUFFER_SIZE2D, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    glVertexArrayVertexBuffer(m_appSurface,0,m_VBO,0, VERTEX_SIZE2D);

    glCreateBuffers(1, &m_ModelMatricesSSBO);
    glNamedBufferStorage(m_ModelMatricesSSBO, 32000 * sizeof(glm::mat4), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    m_vboBase = glMapNamedBufferRange(m_VBO, 0, BUFFER_SIZE2D, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    m_mmSSBOBase = glMapNamedBufferRange(m_ModelMatricesSSBO, 0, 32000 * sizeof(glm::mat4), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

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
    glVertexArrayElementBuffer(m_appSurface, m_IBO);


    for(int i=0;i<5;i++){
        glEnableVertexArrayAttrib(m_appSurface,i);
        glVertexArrayAttribBinding(m_appSurface,i,0);
    }
    glVertexArrayAttribFormat(m_appSurface, 0, 3, GL_FLOAT, GL_FALSE, offsetof(vboData, pos));
    glVertexArrayAttribFormat(m_appSurface, 1, 4, GL_FLOAT, GL_FALSE, offsetof(vboData, color));
    glVertexArrayAttribFormat(m_appSurface, 2, 2, GL_FLOAT, GL_FALSE, offsetof(vboData, texCoords));
    glVertexArrayAttribFormat(m_appSurface, 3, 1, GL_INT, GL_FALSE, offsetof(vboData, textureSlot));
    glVertexArrayAttribFormat(m_appSurface, 4, 1, GL_UNSIGNED_INT, GL_FALSE, offsetof(vboData, modelID));
    
    m_Shader=Engine::CreateShader(Engine::LoadShaderFromFile("./Graphics/Shaders/shader.vs").c_str(),
                                  Engine::LoadShaderFromFile("./Graphics/Shaders/shader.fs").c_str());
}
/**
 * @brief Frees the buffers in memory
 * 
 */
Renderer2D::~Renderer2D() {
    glDeleteBuffers(1,&m_IBO);
    glDeleteBuffers(1,&m_VBO);
    glDeleteBuffers(1,&m_ModelMatricesSSBO);
    glDeleteProgram(m_Shader);
}

/**
 * @brief Start rendering objects added to the renderer
 * 
 */
void Renderer2D::Render(){
    GLsizei m_indexCount=0, m_instanceCount = 0;
    vboData* m_Buff = (vboData*) m_vboBase;
    glm::mat4* m_Model = (glm::mat4*) m_mmSSBOBase;

    if(!m_indexCount && m_fence){
        glClientWaitSync(m_fence, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED);
        glDeleteSync(m_fence);
        m_fence=nullptr;
    }

    for(Sprite& spr:SpritesToRender){
        float textureSlot =spr.getTextureSlot();
        m_Model[m_instanceCount] = spr.modelMatrix;
        if(!spr.hasTexture())
            textureSlot=-1;
        glm::vec2& _origin = spr.getOrigin();
        glm::vec2& _size = spr.getSize();
        glm::vec4& _col = spr.getColor();
        std::vector<glm::vec2> uvs = spr.getUV();
        std::vector<glm::vec3> _box;
        _box.push_back(glm::vec3(_origin.x,_origin.y,0));
        _box.push_back(glm::vec3(_size.x+_origin.x,_origin.y,0));
        _box.push_back(glm::vec3(_size.x+_origin.x,_size.y+_origin.y,0));
        _box.push_back(glm::vec3(_origin.x,_size.y+_origin.y,0));
        for(int i=0;i<4;i++){
            m_Buff->texCoords = uvs[i];
            m_Buff->pos = _box[i];
            m_Buff->color=_col;
            m_Buff->textureSlot = textureSlot;
            m_Buff->modelID = m_instanceCount;
            m_Buff++;
        }
        m_indexCount+=6;
        m_instanceCount++;
    }
    
    SpritesToRender.clear();
    
    glUseProgram(m_Shader);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ModelMatricesSSBO);

    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "proj_matrix"),1,GL_FALSE,Engine::getOrthoMatrix());
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "vw_matrix"),1,GL_FALSE,Engine::getViewMatrix2D());
    glUniform1i(glGetUniformLocation(m_Shader, "texture[0]"), MainTextureAtlas.GetTextureSlot());
    MainTextureAtlas.Bind();
    glBindVertexArray(m_appSurface);
    //glDisable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, NULL);
    //glDepthMask(GL_TRUE);
    //glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
    m_fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    MainTextureAtlas.Unbind();
    glUseProgram(0);
}
