#ifndef GAME_SPRITE
#define GAME_SPRITE

#include "../Engine.h"
#include "stb_image.h"
#include <map>
#include <functional>
class Sprite{

public:
    Sprite(const char*, glm::vec3);
	Sprite(glm::vec3, glm::vec2);
    void setPosition(glm::vec3);
    void setScale(glm::vec2);
    void setColor(glm::vec4);
    void setFrameIndex(unsigned);
    inline unsigned getFrameCount(){return m_Frames;}
    inline int getTextureSlot(){return m_texSlot;}
	inline glm::vec3& getPosition(){return m_Pos;}
    inline glm::vec2& getSize(){return m_Size;}
    inline glm::vec2 getSumSize(){return glm::vec2(m_WidthCombined, m_HeightCombined);}
    inline glm::vec2& getBaseSize(int _frameIndex=-1){if(_frameIndex==-1) _frameIndex=m_FrameIndex;return m_BaseSize[_frameIndex];}
    inline glm::vec4& getColor(){return m_Color;}
    inline std::vector<glm::vec2>& getUV(int _frameIndex=-1){if(_frameIndex==-1) _frameIndex=m_FrameIndex;return m_UV[_frameIndex];}
    inline bool& hasTexture(){return m_hasTexture;}

    friend class TexturePage;
    friend std::vector<unsigned char*> LoadNGESprite(const char*, Sprite*);

    Sprite& operator=(const Sprite& other){
        this->m_Name = other.m_Name;
        this->m_Frames = other.m_Frames;
        this->m_FrameIndex = other.m_FrameIndex;
        this->m_Pos = other.m_Pos;
        this->m_BaseSize = other.m_BaseSize;
        this->m_Size = other.m_Size;
        this->m_Color = other.m_Color;
        this->m_hasTexture = other.m_hasTexture;
        this->m_texSlot = other.m_texSlot;

        this->m_UV = other.m_UV;
        return *this;
    
    }
    bool operator==(const Sprite&) const;

    /*void setUV(){
        std::vector<glm::vec2> UV;
        UV.push_back(glm::vec2(0));
        UV.push_back(glm::vec2(0,1));
        UV.push_back(glm::vec2(1,1));
        UV.push_back(glm::vec2(1,0));
        m_UV.push_back(UV);
        m_hasTexture=true;
        m_texSlot=0;
    }*/
protected:
    std::string m_Name;
    unsigned int m_Frames;
    unsigned int m_FrameIndex;
    glm::vec3 m_Pos;
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
    glm::vec2 TextureAdd (std::vector<unsigned char>, unsigned, unsigned);
	void Bind();
	void Unbind();
	int GetTextureSlot();
    glm::vec2 GetAtlasSize();
    // void setTexture(unsigned int _texture){m_Texture = _texture;}
};
extern std::map<std::string, Sprite*> SpritesTotal;
extern TexturePage MainTextureAtlas;
#endif
