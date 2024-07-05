#ifndef GAME_SPRITE
#define GAME_SPRITE

#include "../Engine.h"
#include "stb_image.h"
#include <map>
#include <functional>
class Sprite{
public:
    Sprite(const char*, glm::vec2);
    Sprite(const char*);
	Sprite(glm::vec2, glm::vec2);
    void DeleteSprite();
    void setPosition(glm::vec2);
    void setScale(glm::vec2);
    void setColor(glm::vec4);
    inline unsigned getFrameCount(){return m_Frames;}
    inline int getTextureSlot(){return m_texSlot;}
    inline unsigned char* getPixels(){return m_Pixels;}
	inline glm::vec2& getPosition(unsigned int _frameIndex=m_FrameIndex){return m_Pos[_frameIndex];}
    inline glm::vec2& getSize(){return m_Size}
    inline glm::vec2& getBaseSize(unsigned int _frameIndex=m_FrameIndex){return m_BaseSize[_frameIndex];}
    inline glm::vec4& getColor(){return m_Color;}
    inline std::vector<glm::vec2>& getUV(unsigned int _frameIndex=m_FrameIndex){return m_UV[_frameIndex];}
    inline bool& hasTexture(){return m_hasTexture;}

    friend class TexturePage;
    bool operator==(const Sprite&) const;
protected:
    std::vector<char*> LoadNGESprite(const char*); 
    std::string m_Name;
    unsigned int m_FrameIndex;
    unsigned int m_Frames;
    std::vector<glm::vec2> m_Pos;
    std::vector<glm::vec2> m_BaseSize;
    glm::vec2 m_Size;
    glm::vec4 m_Color;
    
    bool m_hasTexture;
    unsigned char* m_Pixels;
    int m_texSlot;
    
    std::vector<std::vector<glm::vec2>> m_UV;
    unsigned long long m_Widest, m_Heighest, m_HeightCombined;
	
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
    void ImageAdd(unsigned char*, std::vector<glm::vec2>&, glm::vec2, int&);
	void Bind();
	void Unbind();
	int GetTextureSlot();
    glm::vec2 GetAtlasSize();
};
extern std::map<std::string, Sprite*> SpritesTotal;
extern TexturePage MainTextureAtlas;
#endif
