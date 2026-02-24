#include "Sprite.h"
std::map<std::string, Sprite*> SpritesTotal;
TexturePage MainTextureAtlas(8000,8000,4,0);

/**
 * @brief Create a sprite without texture
 * 
 * @param _pos position where to render the sprite
 * @param _size size of the sprite
 */
Sprite::Sprite(): m_Name(""),m_Frames(0), m_FrameIndex(0),m_Origin(glm::vec2(0,0)), m_hasTexture(false), m_texSlot(-1), m_Widest(0), m_Heighest(0),m_WidthCombined(0), m_HeightCombined(0){
}
void Sprite::setOrigin(glm::vec2 origin){
    m_Origin = origin;
}
void Sprite::setColor(glm::vec4 color){
    m_Color = color;
}
void Sprite::setFrameIndex(unsigned _index){
    m_FrameIndex = _index%m_Frames;
}

/**
 * @brief Initialize the texture page where the sprites should be saved
 * 
 * @param slot texture slot to save in between 0 and 31 (default = 0)
 */
TexturePage::TexturePage(int slot) :m_xOffset(0),m_yOffset(0), m_Width(0), m_Height(0), m_ChannelNum(4), m_Slot(slot){
	m_TexturePage = (unsigned char*)malloc(1);
    glGenTextures(1, &m_Texture);
}
/**
 * @brief Overloaded constructor to initialize the texture page where the sprites should be saved
 * 
 *
 * @param width Width of the texture page to generate
 * @param height Height of the texture page to generate
 * @param channel_num number of channels of the texture page (default = 4)
 * @param slot texture slot to save in between 0 and 31 (default = 0)
 */
TexturePage::TexturePage(int _width, int _height, int _channel_num, int _slot): m_xOffset(0),m_yOffset(0),m_Width(_width), m_Height(_height), m_ChannelNum(_channel_num),m_lastYOffset(0), m_Slot(_slot){
	m_TexturePage = (unsigned char*)malloc(m_Width*m_Height*m_ChannelNum);
    glGenTextures(1, &m_Texture);
}
/**
 * @brief Resize the texture page so that it can take more sprites
 * 
 * @param new_width Width of the texture page
 * @param new_height Height of the texture page
 * @param channel_num Number of channels (4 is default)
 */
void TexturePage::ImageResizeCanvas(int new_width, int new_height, int channel_num) {
    
    m_TexturePage = (unsigned char*)realloc(m_TexturePage,new_width*new_height*channel_num);
    /*
    std::map<std::string, Sprite>::iterator it;
    for (it=SpritesTotal.begin();it!=SpritesTotal.end();it++){
        Sprite* spr = &it->second;
        for(unsigned i=0;i<spr->getFrameCount();i++){
            float x1= 0,
                y1= (float)spr->m_UV[i][0].y*m_Height/new_height,
                x2= (float) spr->getBaseSize().x / new_width,
                y2=y1+ (float) spr->getBaseSize().y / new_height;
            spr->m_UV[i][0]=glm::vec2(x1,y1);
            spr->m_UV[i][1]=glm::vec2(x2,y1);
            spr->m_UV[i][2]=glm::vec2(x2,y2);
            spr->m_UV[i][3]=glm::vec2(x1,y2);
        }
    }
    */
    m_ChannelNum = channel_num;
    m_Width = new_width;
    m_Height = new_height;
}
/**
 * @brief Add image to texture page
 * 
 * @param data pixels data
 * @param spr The sprite object
 * @param t_Frame Frame index of the sprite (for animations)
 */
void TexturePage::ImageAdd(unsigned char* data, Sprite* spr, int t_Frame){
    float width = spr->m_CurrentFrameSize[t_Frame].x;
	float height = spr->m_CurrentFrameSize[t_Frame].y;
    if((m_xOffset+width)>=m_Width){
        m_xOffset=0;
        m_yOffset+=m_lastYOffset;
        m_lastYOffset=0;
    }
    if(height>=m_lastYOffset)
        m_lastYOffset = height;
    
    float x1=m_xOffset/m_Width,
          y1=m_yOffset/m_Height,
          x2= x1+ width /m_Width,
          y2= y1+ height /m_Height;
    /*
    UV.push_back(glm::vec2(0,0));
    UV.push_back(glm::vec2(1,0));
    UV.push_back(glm::vec2(1,1));
    UV.push_back(glm::vec2(0,1));
    */
    spr->m_UV[t_Frame][0]=glm::vec2(x1,y1);
    spr->m_UV[t_Frame][1]=glm::vec2(x2,y1);
    spr->m_UV[t_Frame][2]=glm::vec2(x2,y2);
    spr->m_UV[t_Frame][3]=glm::vec2(x1,y2);
    //std::cout << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
    spr->m_texSlot=m_Slot;
    
    for (unsigned int y = 0;y < height;y++) {
        for (unsigned int x = 0;x < width;x++) {
            for(int i=0;i<m_ChannelNum;i++){
                m_TexturePage[(unsigned)(((x+ m_xOffset) + (y+m_yOffset)*m_Height) * m_ChannelNum + i)] = data[(unsigned)((x + width * y) * m_ChannelNum + i)];
            }
        }
    }
    m_xOffset+=width;
   
    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_TexturePage);
    Unbind();
    
}
glm::vec2 TexturePage::TextureAdd(std::vector<unsigned char> data, unsigned width, unsigned height){
    
    if((m_xOffset+width)>=m_Width){
        m_xOffset=0;
        m_yOffset+=m_lastYOffset;
        m_lastYOffset=0;
    }
    if(height>=m_lastYOffset)
        m_lastYOffset = height;
    
    /*float x1=m_xOffset/m_Width,
          y1=m_yOffset/m_Height,
          x2= x1+ width /m_Width,
          y2= y1+ height /m_Height;*/
          
    
    glm::vec2 coordsOffset(m_xOffset/m_Width, m_yOffset/m_Height);
    for (unsigned int y = 0;y < height;y++) {
        for (unsigned int x = 0;x < width;x++) {
            for(int i=0;i<m_ChannelNum;i++){
                m_TexturePage[(unsigned)(((x+ m_xOffset) + (y+m_yOffset)*m_Height) * m_ChannelNum + i)] = data[(unsigned)((x + width * y) * m_ChannelNum + i)];
            }
        }
    }
    m_xOffset+=width;
   
    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    if(m_ChannelNum==4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_TexturePage);
    else if(m_ChannelNum==1)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE, m_TexturePage);
    Unbind();
    
    //stbi_write_png("atlas_debug.png", m_Width, m_Height, 1, m_TexturePage, m_Width);
    return coordsOffset;
}
/**
 
void TexturePage::ImageAdd(Sprite& sprite) {
	if(sprite.hasTexture()){
		unsigned char* data = sprite.m_Pixels;
		int width = sprite.m_BaseSize.x;
		int height = sprite.m_BaseSize.y;
        
		float x1=0,
              y1= (float)m_yOffset/m_Height,
			  x2= (float) width /m_Width,
			  y2=y1+(float) height /m_Height;
		sprite.m_UV[0]=glm::vec2(x1, y1); // 0, 0
		sprite.m_UV[1]=glm::vec2(x2, y1); // 1, 0
		sprite.m_UV[2]=glm::vec2(x2, y2); // 1, 1
		sprite.m_UV[3]=glm::vec2(x1, y2); // 0, 1
		
		sprite.m_texSlot=m_Slot;
        
		for (int x = 0;x < width;x++) {
			for (int y = 0;y < height;y++) {
				m_TexturePage[(x+ m_Width * (y+m_yOffset)) * m_ChannelNum + 0] = data[(x + width * y) * m_ChannelNum + 0];
				m_TexturePage[(x+ m_Width * (y+m_yOffset)) * m_ChannelNum + 1] = data[(x + width * y) * m_ChannelNum + 1];
				m_TexturePage[(x+ m_Width * (y+m_yOffset)) * m_ChannelNum + 2] = data[(x + width * y) * m_ChannelNum + 2];
				m_TexturePage[(x+ m_Width * (y+m_yOffset)) * m_ChannelNum + 3] = data[(x + width * y) * m_ChannelNum + 3];
			}
		}
        m_yOffset+=height;
		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_TexturePage);
		Unbind();
	}
    SpritesTotal.insert({sprite.m_Name, &sprite});
}
*/
/**
 * @brief Bind the texture to one of slots in memory (usually between 0 and 31)
 * 
 */
void TexturePage::Bind() {
    glActiveTexture(GL_TEXTURE0+m_Slot);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
}
/**
 * @brief Unbind the texture after loading the sprite
 * 
 */
void TexturePage::Unbind() {
    glActiveTexture(GL_TEXTURE0 + m_Slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}
/**
 * @brief Return the value of the current texture page slot
 * 
 * @return int slot of the current texture page
*/
int TexturePage::GetTextureSlot(){
	return m_Slot;
}
/**
 * @brief Return the value of the current texture page size
 * 
 * @return glm::vec2 size of the current texture page
*/
glm::vec2 TexturePage::GetAtlasSize(){
    return glm::vec2(m_Width, m_Height);
}