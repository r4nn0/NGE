#include "TexturePage.h"
TexturePage::TexturePage() {
    m_xOffset = 0;
    m_yOffset= 0;
    m_Width = 0;
    m_Height = 0;
    m_ChannelNum = 4;
    glGenTextures(1, &m_Texture);
}
void TexturePage::ImageResizeCanvas(int new_width, int new_height, int channel_num=4) {
    unsigned char *data = m_TexturePage;
    m_TexturePage = (unsigned char*)malloc(new_width * new_height * channel_num);
    m_ChannelNum = channel_num;
    for (int x = 0;x < new_width;x++) {
        for (int y = 0;y < new_height;y++) {
            if (x < m_Width && y < m_Height) {
                m_TexturePage[(x + new_width * y) * channel_num + 0] = data[(x + m_Width * y) * channel_num + 0];
                m_TexturePage[(x + new_width * y) * channel_num + 1] = data[(x + m_Width * y) * channel_num + 1];
                m_TexturePage[(x + new_width * y) * channel_num + 2] = data[(x + m_Width * y) * channel_num + 2];
                m_TexturePage[(x + new_width * y) * channel_num + 3] = data[(x + m_Width * y) * channel_num + 3];
            }
            else {
                m_TexturePage[(x + new_width * y) * channel_num + 0] = 255;
                m_TexturePage[(x + new_width * y) * channel_num + 1] = 255;
                m_TexturePage[(x + new_width * y) * channel_num + 2] = 255;
                m_TexturePage[(x + new_width * y) * channel_num + 3] = 0;
            }
        }
    }
    m_Width = new_width;
    m_Height = new_height;
}
void TexturePage::ImageAdd(Sprite* sprite) {
	if(sprite->hasTexture()){
		unsigned char* data = sprite->getPixels();
		int width = sprite->getBaseSize().x;
		int height = sprite->getBaseSize().y;
		std::vector<glm::vec2> uvs;
		float x1= (float)m_xOffset/m_Width,
			  y1= (float)m_yOffset/m_Height,
			  x2=x1+(float) width /m_Width,
			  y2=y1+ (float)height /m_Height;
		uvs.push_back(glm::vec2(x1, y1)); // 0, 0
		uvs.push_back(glm::vec2(x2, y1)); // 1, 0
		uvs.push_back(glm::vec2(x2, y2)); // 1, 1
		uvs.push_back(glm::vec2(x1, y2)); // 0, 1
		sprite->setUV(uvs);
		if (m_xOffset >= m_Width || m_yOffset >= m_Height) return;
		for (int x = 0;x < width;x++) {
			for (int y = 0;y < height;y++) {
				m_TexturePage[(x+ m_xOffset + m_Width * (y+ m_yOffset)) * m_ChannelNum + 0] = data[(x + width * y) * m_ChannelNum + 0];
				m_TexturePage[(x+ m_xOffset + m_Width * (y+ m_yOffset)) * m_ChannelNum + 1] = data[(x + width * y) * m_ChannelNum + 1];
				m_TexturePage[(x+ m_xOffset + m_Width * (y+ m_yOffset)) * m_ChannelNum + 2] = data[(x + width * y) * m_ChannelNum + 2];
				m_TexturePage[(x+ m_xOffset + m_Width * (y+ m_yOffset)) * m_ChannelNum + 3] = data[(x + width * y) * m_ChannelNum + 3];
			}
		}
		m_xOffset += width;
		m_yOffset += 0;
		Bind(m_Slot);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_TexturePage);
	}
}
void TexturePage::Bind(int slot = 0) {
    m_Slot = slot;
    glActiveTexture(GL_TEXTURE0+slot);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
}
void TexturePage::Unbind() {
    glActiveTexture(GL_TEXTURE0 + m_Slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}
glm::vec2 TexturePage::getOffset() {
    return glm::vec2(m_xOffset, m_yOffset);
}