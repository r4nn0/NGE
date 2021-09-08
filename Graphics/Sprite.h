#ifndef GAME_SPRITE
#define GAME_SPRITE

#include "../Engine.h"
#include "stb_image.h"

class Sprite{
public:
    Sprite(const char* path, glm::vec3);

    void Bind();
    void Unbind();

    void setPosition(glm::vec3);
    void setScale(glm::vec2);
    void setColor(glm::vec4);

    unsigned int& getTexture(){return m_Texture;}
    int& getPixels(){return m_BPP;}
	glm::vec3& getPosition(){return m_Pos;}
    glm::vec2& getSize(){return m_Size;}
    glm::vec4& getColor(){return m_Color;}
protected:
    glm::vec3 m_Pos;
    glm::vec2 m_Size;
    glm::vec2 m_BaseSize;
    glm::vec4 m_Color;
    int m_BPP;
    unsigned char* m_Pixels;
	unsigned int m_Texture;
};

#endif
