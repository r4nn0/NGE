#ifndef GAME_SPRITE
#define GAME_SPRITE

#include "../Engine.h"
#include "stb_image.h"
#include <functional>
class Sprite{
public:
    Sprite(const char* path, glm::vec3);
	Sprite(glm::vec3, glm::vec2);
    void setPosition(glm::vec3);
    void setScale(glm::vec2);
    void setColor(glm::vec4);
    
    inline int getTextureSlot(){return m_texSlot;}
    inline unsigned char* getPixels(){return m_Pixels;}
	inline glm::vec3& getPosition(){return m_Pos;}
    inline glm::vec2& getSize(){return m_Size;}
    inline glm::vec2& getBaseSize(){return m_BaseSize;}
    inline glm::vec4& getColor(){return m_Color;}
    inline std::vector<glm::vec2>& getUV(){return m_UV;}
    inline bool& hasTexture(){return m_hasTexture;}

    friend class TexturePage;
protected:
    void setUV(std::vector<glm::vec2>);
    void setTextureSlot(int);
    glm::vec3 m_Pos;
    glm::vec2 m_Size;
    glm::vec2 m_BaseSize;
    glm::vec4 m_Color;
    std::vector<glm::vec2> m_UV;
    int m_texSlot;
	bool m_hasTexture;
    unsigned char* m_Pixels;
};
class TexturePage{
private:
	int m_xOffset, m_yOffset ,m_Width, m_Height, m_ChannelNum;
	const int m_Slot;
	unsigned char* m_TexturePage;
	unsigned int m_Texture;
    std::vector<std::reference_wrapper<Sprite>> SpritesTotal;
public:
	TexturePage(int=0);
	TexturePage(int, int, int=4, int=0);
	void ImageResizeCanvas(int, int, int=4);
	void ImageAdd(Sprite&);
	void Bind();
	void Unbind();
	int GetTextureSlot();
    glm::vec2 GetAtlasSize();
};

extern TexturePage MainTextureAtlas;
#endif
