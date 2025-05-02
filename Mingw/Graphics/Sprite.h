#ifndef GAME_SPRITE
#define GAME_SPRITE

#include "../Engine.h"
#include "stb_image.h"
#include <map>
#include <functional>
class Sprite{

public:
    Sprite(const char*, glm::vec2);
	Sprite(glm::vec2, glm::vec2);
    void setPosition(glm::vec2);
    void setScale(glm::vec2);
    void setColor(glm::vec4);
    void setFrameIndex(unsigned);
    inline unsigned getFrameCount(){return m_Frames;}
    inline int getTextureSlot(){return m_texSlot;}
	inline glm::vec2& getPosition(){return m_Pos;}
    inline glm::vec2& getSize(){return m_Size;}
    inline glm::vec2 getSumSize(){return glm::vec2(m_WidthCombined, m_HeightCombined);}
    inline glm::vec2& getBaseSize(int _frameIndex=-1){if(_frameIndex==-1) _frameIndex=m_FrameIndex;return m_BaseSize[_frameIndex];}
    inline glm::vec4& getColor(){return m_Color;}
    inline std::vector<glm::vec2>& getUV(int _frameIndex=-1){if(_frameIndex==-1) _frameIndex=m_FrameIndex;return m_UV[_frameIndex];}
    inline bool& hasTexture(){return m_hasTexture;}

    friend class TexturePage;
    friend std::vector<unsigned char*> LoadNGESprite(const char*, Sprite*);
    bool operator==(const Sprite&) const;


    
protected:
    std::string m_Name;
    unsigned int m_Frames;
    unsigned int m_FrameIndex;
    glm::vec2 m_Pos;
    std::vector<glm::vec2> m_BaseSize;
    glm::vec2 m_Size;
    glm::vec4 m_Color;
    
    bool m_hasTexture;
    int m_texSlot;
    
    std::vector<std::vector<glm::vec2>> m_UV;
    unsigned long long m_Widest, m_Heighest, m_WidthCombined, m_HeightCombined;
	
};
class TexturePage{
private:
	float m_xOffset, m_yOffset ,m_Width, m_Height, m_ChannelNum, m_lastYOffset;
	const int m_Slot;
	unsigned char* m_TexturePage;
	unsigned int m_Texture;
public:
	TexturePage(int=0);
	TexturePage(int, int, int=4, int=0);
	void ImageResizeCanvas(int, int, int=4);
    void ImageAdd(unsigned char*, Sprite*, int);
	void Bind();
	void Unbind();
	int GetTextureSlot();
    glm::vec2 GetAtlasSize();
};
extern std::map<std::string, Sprite*> SpritesTotal;
extern TexturePage MainTextureAtlas;
#endif
