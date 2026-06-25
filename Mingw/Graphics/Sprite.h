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
    inline std::vector<glm::vec2>& getFramesSize(){return m_FramesSize;}
    inline int& getTextureSlot(){return m_texSlot;}
    inline std::vector<std::vector<glm::vec2>>& getUVs() {return m_UVs;}
    inline std::vector<std::vector<bool>>& getCollisionMask() {return collisionMask;}
    

    friend class TexturePage;
    friend std::vector<unsigned char*> LoadNGESprite(const char*, Sprite*);

protected:
    std::vector<glm::vec2> m_FramesSize;
    int m_texSlot;
    std::vector<std::vector<glm::vec2>> m_UVs;
    std::vector<std::vector<bool>> collisionMask;
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
