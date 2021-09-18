#include "Sprite.h"
/// Constructors
Sprite::Sprite(const char* path, glm::vec3 _pos): m_Pos(_pos){
    int _width, _height, _bpp;
    m_Pixels= stbi_load(path, &_width, &_height, &_bpp, 4);
    m_BaseSize=glm::vec2(_width, _height);
    m_Size=m_BaseSize;
    m_Color=glm::vec4(1,1,1,1);
    m_UV.push_back(glm::vec2(0, 0));
    m_UV.push_back(glm::vec2(1, 0));
    m_UV.push_back(glm::vec2(1, 1));
    m_UV.push_back(glm::vec2(0, 1));
}

void Sprite::setUV(std::vector<glm::vec2> new_uv) {
    m_UV[0] = new_uv[0];
    m_UV[1] = new_uv[1];
    m_UV[2] = new_uv[2];
    m_UV[3] = new_uv[3];
}
inline void Sprite::setPosition(glm::vec3 _pos) {m_Pos=_pos;}
void Sprite::setScale(glm::vec2 _scale)  {m_Size=_scale*m_BaseSize;}
inline void Sprite::setColor(glm::vec4 _color)  {m_Color=_color;}

