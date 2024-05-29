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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (const void*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (const void*)(sizeof(float)*3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (const void*)(sizeof(float)*7));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (const void*)(sizeof(float)*9));
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

}
/**
 * @brief Frees the buffers in memory
 * 
 */
Renderer2D::~Renderer2D()
{
    delete m_indexBuffer;
    glDeleteBuffers(1,&m_VBO);
}
/**
 * @brief Initializes rendering, necessary before adding sprites
 * 
 */
void Renderer2D::renderBegin(){
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_Buff = (ngetype::vboData*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}
/**
 * @brief Ends rendering so that the engine knows it is ready to render
 * 
 */
void Renderer2D::renderEnd(){
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
/**
 * @brief Add a sprite to the rendering screen
 * 
 * @param spr the object of the sprite to add
 */
void Renderer2D::addSprite(Sprite* spr){
	float texID =m_textureCount;
	if(!spr->hasTexture())
		texID=-1;
    glm::vec3& _pos = spr->getPosition();
    glm::vec2& _size = spr->getSize();
    glm::vec4& _col = spr->getColor();
    std::vector<glm::vec2> uvs = spr->getUV();
    //m_Buff->texCoords = glm::vec2(0, 0);
    m_Buff->texCoords = uvs[0];
    m_Buff->vertex=_pos;
    m_Buff->color=_col;
    m_Buff->texID=texID;
    m_Buff++;

    //m_Buff->texCoords = glm::vec2(1, 0);
    m_Buff->texCoords = uvs[1];
    m_Buff->vertex=glm::vec3(_pos.x+_size.x,_pos.y,_pos.z);
    m_Buff->color=_col;
    m_Buff->texID = texID;
    m_Buff++;

    //m_Buff->texCoords = glm::vec2(1, 1);
    m_Buff->texCoords = uvs[2];
    m_Buff->vertex=glm::vec3(_pos.x+_size.x,_pos.y+_size.y,_pos.z);
    m_Buff->color=_col;
    m_Buff->texID = texID;
    m_Buff++;

    //m_Buff->texCoords = glm::vec2(0, 1);
    m_Buff->texCoords = uvs[3];
    m_Buff->vertex=glm::vec3(_pos.x,_pos.y+_size.y,_pos.z);
    m_Buff->color=_col;
    m_Buff->texID = texID;
    m_Buff++;
    m_indexCount+=6;
    m_textureCount++;
}
/**
 * @brief Start rendering objects added to the renderer
 * 
 */
void Renderer2D::Render(){
    glBindVertexArray(m_appSurface);
    m_indexBuffer->bind();
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, NULL);
    m_indexBuffer->unbind();
    glBindVertexArray(0);
    m_indexCount = 0;
    m_textureCount = 0;
    dcpf++;
}
