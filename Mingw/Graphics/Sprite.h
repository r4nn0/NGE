#ifndef GAME_SPRITE
#define GAME_SPRITE

#include "../Engine.h"
#include "stb_image.h"

class Sprite{
public:
    Sprite(const char* path, glm::vec3);
	Sprite(glm::vec3, glm::vec2);
    void setPosition(glm::vec3);
    void setScale(glm::vec2);
    void setUV(std::vector<glm::vec2>);
    void setColor(glm::vec4);

    
    inline unsigned char* getPixels(){return m_Pixels;}
	inline glm::vec3& getPosition(){return m_Pos;}
    inline glm::vec2& getSize(){return m_Size;}
    inline glm::vec2& getBaseSize(){return m_BaseSize;}
    inline glm::vec4& getColor(){return m_Color;}
    inline std::vector<glm::vec2>& getUV(){return m_UV;}
    inline bool& hasTexture(){return m_hasTexture;}
protected:
    glm::vec3 m_Pos;
    glm::vec2 m_Size;
    glm::vec2 m_BaseSize;
    glm::vec4 m_Color;
    std::vector<glm::vec2> m_UV;
	bool m_hasTexture;
    unsigned char* m_Pixels;
};

#endif
