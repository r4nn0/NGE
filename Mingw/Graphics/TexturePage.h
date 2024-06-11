#include "Sprite.h"
class TexturePage
{
private:
	int m_xOffset, m_yOffset, m_Width, m_Height, m_ChannelNum, m_Slot;
	std::vector<int> m_yOffsetRow;
	unsigned char* m_TexturePage;
	unsigned int m_Texture;
public:
	TexturePage(int=0);
	TexturePage(int, int, int=4, int=0);
	void ImageResizeCanvas(int, int, int=4);
	void ImageAdd(Sprite*);
	void Bind();
	void Unbind();
	int GetTextureSlot();
};

