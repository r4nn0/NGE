#include "Sprite.h"
/**
 * @brief Create a new sprite from an image
 * 
 * @param path Path to image
 * @param _pos position where to render the sprite
 */
Sprite::Sprite(const char* path, glm::vec3 _pos): m_Pos(_pos){
    int _width, _height, _bpp;
    m_Pixels= stbi_load(path, &_width, &_height, &_bpp, 4);
    if(m_Pixels==nullptr){
        std::cout << "Error loading sprite from file" << std::endl;
        //return;
    }
	m_hasTexture=true;
    m_BaseSize=glm::vec2(_width, _height);
    m_Size=m_BaseSize;
    m_Color=glm::vec4(1,1,1,1);
    m_UV.push_back(glm::vec2(0, 0));
    m_UV.push_back(glm::vec2(1, 0));
    m_UV.push_back(glm::vec2(1, 1));
    m_UV.push_back(glm::vec2(0, 1));
}
/**
 * @brief Create a sprite without texture
 * 
 * @param _pos position where to render the sprite
 * @param _size size of the sprite
 */
Sprite::Sprite(glm::vec3 _pos, glm::vec2 _size): m_Pos(_pos){
	m_hasTexture=false;
	m_BaseSize=_size;
	m_Size=m_BaseSize;
    m_Color=glm::vec4(1,1,1,1);
	m_UV.push_back(glm::vec2(0, 0));
    m_UV.push_back(glm::vec2(1, 0));
    m_UV.push_back(glm::vec2(1, 1));
    m_UV.push_back(glm::vec2(0, 1));
}
/**
 * @brief Sets the UV Coordinates of the sprite for the texture page
 * 
 * @param new_uv Texture coordinates
 */
void Sprite::setUV(std::vector<glm::vec2> new_uv) {
    m_UV[0] = new_uv[0];
    m_UV[1] = new_uv[1];
    m_UV[2] = new_uv[2];
    m_UV[3] = new_uv[3];
}
/**
 * @brief Set poisition of the sprite
 * 
 * @param _pos position where the sprite should be rendered
 */
void Sprite::setPosition (glm::vec3 _pos) {m_Pos=_pos;}
/**
 * @brief Scales the sprite
 * 
 * @param _scale scale in vec2, where (1,1) is the base scale
 */
void Sprite::setScale(glm::vec2 _scale)  {m_Size=_scale*m_BaseSize;}
/**
 * @brief Set the color in which the sprite will be rendered
 * 
 * @param _color Color in RGBA format
 */
void Sprite::setColor(glm::vec4 _color)  {m_Color=_color;}

