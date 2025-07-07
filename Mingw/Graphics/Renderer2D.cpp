#include "Renderer2D.h"
#define VERTEX_SIZE2D sizeof(vboData)
#define BUFFER_SIZE2D VERTEX_SIZE2D * 4 * 32000
#define INDICES_SIZE2D 32000*6
/**
 * @brief Initialize batch renderer to pass correct values to shaders
 * 
 */
Renderer2D::Renderer2D() {
    glCreateVertexArrays(1, &m_appSurface);

    glCreateBuffers(1, &m_VBO);
    glNamedBufferStorage(m_VBO, BUFFER_SIZE2D, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
    glVertexArrayVertexBuffer(m_appSurface,0,m_VBO,0, VERTEX_SIZE2D);

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
    glNamedBufferStorage(m_IBO, INDICES_SIZE2D*sizeof(short), indices, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(m_appSurface, m_IBO);

    for(int i=0;i<4;i++){
        glEnableVertexArrayAttrib(m_appSurface,i);
        glVertexArrayAttribBinding(m_appSurface,i,0);
    }
    glVertexArrayAttribFormat(m_appSurface, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(m_appSurface, 1, 4, GL_FLOAT, GL_FALSE, sizeof(float)*3);
    glVertexArrayAttribFormat(m_appSurface, 2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*7);
    glVertexArrayAttribFormat(m_appSurface, 3, 1, GL_FLOAT, GL_FALSE, sizeof(float)*9);
    
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
    glDeleteProgram(m_Shader);
}

/**
 * @brief Start rendering objects added to the renderer
 * 
 */
void Renderer2D::Render(){
    GLsizei m_indexCount=0;
    vboData* m_Buff = (vboData*) glMapNamedBuffer(m_VBO, GL_WRITE_ONLY);
    for(Sprite& spr:SpritesToRender){
        float textureSlot =spr.getTextureSlot();
        
        if(!spr.hasTexture())
            textureSlot=-1;
        glm::vec3& _pos = spr.getPosition();
        glm::vec2& _size = spr.getSize();
        glm::vec4& _col = spr.getColor();
        std::vector<glm::vec2> uvs = spr.getUV();
        std::vector<glm::vec3> _box;
        _box.push_back(glm::vec3(0,0,0));
        _box.push_back(glm::vec3(_size.x,0,0));
        _box.push_back(glm::vec3(_size.x,_size.y,0));
        _box.push_back(glm::vec3(0,_size.y,0));
        for(int i=0;i<4;i++){
            m_Buff->texCoords = uvs[i];
            m_Buff->pos = _pos+_box[i];
            m_Buff->color=_col;
            m_Buff->textureSlot = textureSlot;
            m_Buff++;
        }
        m_indexCount+=6;
    }
    glUnmapNamedBuffer(m_VBO);
    SpritesToRender.clear();
    
    glUseProgram(m_Shader);
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "proj_matrix"),1,GL_FALSE,Engine::getOrthoMatrix());
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "vw_matrix"),1,GL_FALSE,Engine::getViewMatrix2D());
    glUniform1i(glGetUniformLocation(m_Shader, "texture[0]"), MainTextureAtlas.GetTextureSlot());
    MainTextureAtlas.Bind();
    glBindVertexArray(m_appSurface);

    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, NULL);
    
    glBindVertexArray(0);
    
    MainTextureAtlas.Unbind();
    glUseProgram(0);
}
