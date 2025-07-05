#include "Renderer2D.h"

/**
 * @brief Initialize batch renderer to pass correct values to shaders
 * 
 */
Renderer2D::Renderer2D() {
    //glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxTextures);
    glCreateVertexArrays(1, &m_appSurface);

    glCreateBuffers(1, &m_VBO);
    glNamedBufferStorage(m_VBO, BUFFER_SIZE, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
    glVertexArrayVertexBuffer(m_appSurface,0,m_VBO,0, VERTEX_SIZE);

    unsigned short indices[INDICES_SIZE];
    unsigned offset=0;
    for(unsigned i=0;i<INDICES_SIZE;i+=6){
        indices[ i ]=offset;
        indices[i+1]=offset+1;
        indices[i+2]=offset+2;
        indices[i+3]=offset+2;
        indices[i+4]=offset+3;
        indices[i+5]=offset;
        offset+=4;
    }
    glCreateBuffers(1, &m_IBO);
    glNamedBufferStorage(m_IBO, INDICES_SIZE*sizeof(short), indices, GL_DYNAMIC_STORAGE_BIT);
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
    //glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    ngetype::vboData* m_Buff = (ngetype::vboData*) glMapNamedBuffer(m_VBO, GL_WRITE_ONLY);
    for(Sprite& spr:SpritesToRender){
        float textureSlot =spr.getTextureSlot();
        
        if(!spr.hasTexture())
            textureSlot=-1;
        glm::vec3& _pos = spr.getPosition();
        glm::vec2& _size = spr.getSize();
        glm::vec4& _col = spr.getColor();
        std::vector<glm::vec2> uvs = spr.getUV();
        //std::cout << uvs[2].x << " " << uvs[2].y << std::endl;
        

        //m_Buff->texCoords = glm::vec2(0, 0);
        m_Buff->texCoords = uvs[0];
        m_Buff->vertex=_pos;
        m_Buff->color=_col;
        m_Buff->textureSlot=textureSlot;
        m_Buff++;

        //m_Buff->texCoords = glm::vec2(1, 0);
        m_Buff->texCoords = uvs[1];
        m_Buff->vertex=glm::vec3(_pos.x+_size.x,_pos.y, _pos.z);
        m_Buff->color=_col;
        m_Buff->textureSlot = textureSlot;
        m_Buff++;

        //m_Buff->texCoords = glm::vec2(1, 1);
        m_Buff->texCoords = uvs[2];
        m_Buff->vertex=glm::vec3(_pos.x+_size.x,_pos.y+_size.y, _pos.z);
        m_Buff->color=_col;
        m_Buff->textureSlot = textureSlot;
        m_Buff++;

        //m_Buff->texCoords = glm::vec2(0, 1);
        m_Buff->texCoords = uvs[3];
        m_Buff->vertex=glm::vec3(_pos.x,_pos.y+_size.y, _pos.z);
        m_Buff->color=_col;
        m_Buff->textureSlot = textureSlot;
        m_Buff++;
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
