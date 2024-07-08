#include "Renderer2D.h"

/**
 * @brief Initialize batch renderer to pass correct values to shaders
 * 
 */
Renderer2D::Renderer2D() : dcpf(0) {
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxTextures);
    glGenVertexArrays(1, &m_appSurface);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_appSurface);
    glBindBuffer(GL_ARRAY_BUFFER,m_VBO);
    glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (const void*)0); // vec2 pos
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (const void*)(sizeof(float)*2)); // vec4 col
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (const void*)(sizeof(float)*6)); // vec2 texcoord
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (const void*)(sizeof(float)*8)); // float textureSlot
    glBindBuffer(GL_ARRAY_BUFFER,0);

    unsigned short indices[INDICES_SIZE];
    unsigned int i, offset=0;
    for(i=0;i<INDICES_SIZE;i+=6){
        indices[ i ]=offset;
        indices[i+1]=offset+1;
        indices[i+2]=offset+2;
        indices[i+3]=offset+2;
        indices[i+4]=offset+3;
        indices[i+5]=offset;
        offset+=4;
    }
    m_indexBuffer = new ngetype::IBO(indices,INDICES_SIZE);
    glBindVertexArray(0);
    m_Shader=Engine::CreateShader(Engine::LoadShaderFromFile("./Graphics/Shaders/shader.vs").c_str(),
                                  Engine::LoadShaderFromFile("./Graphics/Shaders/shader.fs").c_str());
}
/**
 * @brief Frees the buffers in memory
 * 
 */
Renderer2D::~Renderer2D() {
    delete m_indexBuffer;
    glDeleteBuffers(1,&m_VBO);
    glDeleteProgram(m_Shader);
}

/**
 * @brief Start rendering objects added to the renderer
 * 
 */
void Renderer2D::Render(){
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_Buff = (ngetype::vboData*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    for(Sprite& spr:SpritesToRender){
        float textureSlot =spr.getTextureSlot();
        
        if(!spr.hasTexture())
            textureSlot=-1;
        glm::vec2& _pos = spr.getPosition();
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
        m_Buff->vertex=glm::vec2(_pos.x+_size.x,_pos.y);
        m_Buff->color=_col;
        m_Buff->textureSlot = textureSlot;
        m_Buff++;

        //m_Buff->texCoords = glm::vec2(1, 1);
        m_Buff->texCoords = uvs[2];
        m_Buff->vertex=glm::vec2(_pos.x+_size.x,_pos.y+_size.y);
        m_Buff->color=_col;
        m_Buff->textureSlot = textureSlot;
        m_Buff++;

        //m_Buff->texCoords = glm::vec2(0, 1);
        m_Buff->texCoords = uvs[3];
        m_Buff->vertex=glm::vec2(_pos.x,_pos.y+_size.y);
        m_Buff->color=_col;
        m_Buff->textureSlot = textureSlot;
        m_Buff++;
        m_indexCount+=6;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    SpritesToRender.clear();
    
    glUseProgram(m_Shader);
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "proj_matrix"),1,GL_FALSE,Engine::getOthroMatrix());
    glUniform1i(glGetUniformLocation(m_Shader, "texture[0]"), MainTextureAtlas.GetTextureSlot());
    MainTextureAtlas.Bind();
    glBindVertexArray(m_appSurface);
    m_indexBuffer->bind();
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, NULL);
    m_indexBuffer->unbind();
    glBindVertexArray(0);
    m_indexCount = 0;
    dcpf++;
    MainTextureAtlas.Unbind();
    glUseProgram(0);
}
