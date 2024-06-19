#ifndef GAME_SPRITE
#define GAME_SPRITE

#include "../Engine.h"
#include "stb_image.h"
#include <map>
#include <functional>
class Sprite{
public:
    Sprite(const char* path, glm::vec2);
	Sprite(glm::vec2, glm::vec2);
    void DeleteSprite();
    void setPosition(glm::vec2);
    void setScale(glm::vec2);
    void setColor(glm::vec4);
    
    inline int getTextureSlot(){return m_texSlot;}
    inline unsigned char* getPixels(){return m_Pixels;}
	inline glm::vec2& getPosition(){return m_Pos;}
    inline glm::vec2& getSize(){return m_Size;}
    inline glm::vec2& getBaseSize(){return m_BaseSize;}
    inline glm::vec4& getColor(){return m_Color;}
    inline std::vector<glm::vec2>& getUV(){return m_UV;}
    inline bool& hasTexture(){return m_hasTexture;}

    friend class TexturePage;
    bool operator==(const Sprite&) const;
protected:
    std::string m_Name;
    glm::vec2 m_Pos;
    glm::vec2 m_BaseSize;
    glm::vec2 m_Size;
    glm::vec4 m_Color;
    bool m_hasTexture;
    unsigned char* m_Pixels;
    int m_texSlot;
    std::vector<glm::vec2> m_UV;
    
	
};
class TexturePage{
private:
	int m_xOffset, m_yOffset ,m_Width, m_Height, m_ChannelNum;
	const int m_Slot;
	unsigned char* m_TexturePage;
	unsigned int m_Texture;
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
extern std::map<std::string, Sprite*> SpritesTotal;
extern TexturePage MainTextureAtlas;
#endif
