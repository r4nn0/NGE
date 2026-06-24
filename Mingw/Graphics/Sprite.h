#ifndef GAME_SPRITE
#define GAME_SPRITE

#include "../Engine.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <map>
#include <functional>
class Sprite{

public:
	Sprite();
    void setColor(glm::vec4);
    void setFrameIndex(unsigned);
    void setOrigin(glm::vec2);
    void setPosition(glm::vec3);
    void setScale(glm::vec2);
    void setRotation(float);
    inline glm::vec3& getPosition(){return m_Position;}
    inline glm::vec2& getScale(){return m_Scale;}
    inline float& getRotation(){return m_Rotation;}
    inline unsigned& getFrameCount(){return m_Frames;}
    inline int& getTextureSlot(){return m_texSlot;}
	inline glm::vec2& getOrigin(){return m_Origin;}
    inline glm::vec2& getSize(){return m_CurrentFrameSize[m_FrameIndex];}
    inline glm::vec2 getLargestFrame(){return glm::vec2(m_Widest, m_Heighest);}
    inline glm::vec2 getSumSize(){return glm::vec2(m_WidthCombined, m_HeightCombined);}
    inline glm::vec4& getColor(){return m_Color;}
    inline std::vector<glm::vec2>& getUV(){return m_UV[m_FrameIndex];}

    friend class TexturePage;
    friend std::vector<unsigned char*> LoadNGESprite(const char*, Sprite*);

    Sprite& operator=(const Sprite& other){
        this->m_Name = other.m_Name;
        this->m_Frames = other.m_Frames;
        this->m_FrameIndex = other.m_FrameIndex;
        this->m_CurrentFrameSize = other.m_CurrentFrameSize;
        this->m_Color = other.m_Color;
        this->m_texSlot = other.m_texSlot;
        this->m_UV = other.m_UV;
        return *this;
    
    }
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
    std::vector<glm::vec2> m_CurrentFrameSize;
    glm::vec2 m_Origin;
    glm::vec4 m_Color;
    int m_texSlot;
    
    std::vector<std::vector<glm::vec2>> m_UV;
    unsigned long long m_Widest, m_Heighest, m_WidthCombined, m_HeightCombined;
	glm::vec2 m_Scale;
    float m_Rotation;
    glm::vec3 m_Position;
    
};
class TexturePage{
private:
	int m_Width, m_Height, m_ChannelNum;
	const int m_Slot;
	unsigned char* m_TexturePage;
	unsigned int m_Texture;
    bool m_Initiated;
    struct Rect{
        int x, y, width, height;
    };
    std::vector<Rect> m_FreeRects;
    std::vector<Rect> newRects;
public:
	TexturePage(int=0);
	TexturePage(int, int, int=4, int=0);
    void maxRect(unsigned char*, Sprite*, int);
    std::vector<glm::vec2> maxRect(unsigned char*, int, int);
    bool Intersects(const Rect&, const Rect&);
    bool Contains(const Rect&, const Rect&);
    void PruneFreeRects();
    void SplitFreeRect(const Rect&, const Rect&);
	void Bind();
	void Unbind();
	int GetTextureSlot();
    glm::vec2 GetAtlasSize();
    // void setTexture(unsigned int _texture){m_Texture = _texture;}
};
extern std::map<std::string, Sprite*> SpritesTotal;
extern TexturePage MainTextureAtlas;
#endif
