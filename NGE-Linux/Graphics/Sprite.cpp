#include "Sprite.h"
/// Constructors
Sprite::Sprite(const char* path, glm::vec3 _pos): m_Pos(_pos){
    int _width, _height;
    glGenTextures(1, &m_Texture);
    glActiveTexture(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    m_Pixels= stbi_load(path, &_width, &_height, &m_BPP, 4);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height,0,GL_RGBA, GL_UNSIGNED_BYTE, m_Pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    delete m_Pixels;
    m_BaseSize=glm::vec2(_width, _height);
    m_Size=m_BaseSize;
    m_Color=glm::vec4(1,1,1,1);
}
void Sprite::setPosition(glm::vec3 _pos) {m_Pos=_pos;}
void Sprite::setScale(glm::vec2 _scale)  {m_Size=_scale*m_BaseSize;}
void Sprite::setColor(glm::vec4 _color)  {m_Color=_color;}


void Sprite::Bind(){
    glBindTexture(GL_TEXTURE_2D, m_Texture);
}
void Sprite::Unbind(){
    glBindTexture(GL_TEXTURE_2D, 0);
}
