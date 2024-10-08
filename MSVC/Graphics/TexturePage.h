#include "Sprite.h"
class TexturePage
{
private:
	int m_Width, m_Height, m_xOffset, m_yOffset, m_ChannelNum, m_Slot;
	unsigned char* m_TexturePage;
	unsigned int m_Texture;
public:
	TexturePage();
	void ImageResizeCanvas(int, int, int);
	void ImageAdd(Sprite*);
	void Bind(int);
	void Unbind();
	glm::vec2 getOffset();
};

