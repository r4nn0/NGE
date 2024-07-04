#include "Sprite.h"
std::map<std::string, Sprite*> SpritesTotal;
TexturePage MainTextureAtlas;
/**
 * @brief Create a new sprite from an image
 * 
 * @param path Path to image
 * @param _pos position where to render the sprite
 */
Sprite::Sprite(const char* path, glm::vec2 _pos): m_Name(path), m_Pos(_pos),
                                                  m_BaseSize(0), m_Size(0),
                                                  m_Color(1), m_hasTexture(true), m_Pixels(nullptr), m_texSlot(-1){
    int _width, _height, _bpp;
    m_Pixels= stbi_load(path, &_width, &_height, &_bpp, 4);
    if(m_Pixels==nullptr){
        std::cout << "Error loading sprite from file" << std::endl;
        Sprite(_pos, glm::vec2(1));
        return;
    }
	m_hasTexture=true;
    m_BaseSize=glm::vec2(_width, _height);
    m_Size=m_BaseSize;
    m_Color=glm::vec4(1,1,1,1);
    m_UV.push_back(glm::vec2(0, 0));
    m_UV.push_back(glm::vec2(1, 0));
    m_UV.push_back(glm::vec2(1, 1));
    m_UV.push_back(glm::vec2(0, 1));
    glm::vec2 AtlasSize=MainTextureAtlas.GetAtlasSize();
    if(AtlasSize.x<m_BaseSize.x) AtlasSize.x=m_BaseSize.x;
    MainTextureAtlas.ImageResizeCanvas(AtlasSize.x, AtlasSize.y+m_BaseSize.y);
    MainTextureAtlas.ImageAdd(*this);
    stbi_image_free(m_Pixels);
}
/**
 * @brief Create a sprite without texture
 * 
 * @param _pos position where to render the sprite
 * @param _size size of the sprite
 */
Sprite::Sprite(glm::vec2 _pos, glm::vec2 _size): m_Name("noSprite"), m_Pos(_pos),
                                                 m_BaseSize(_size), m_Size(_size),
                                                 m_Color(1), m_hasTexture(false), m_Pixels(nullptr), m_texSlot(-1){
    m_Name+=std::to_string(SpritesTotal.size());
	m_UV.push_back(glm::vec2(0, 0));
    m_UV.push_back(glm::vec2(1, 0));
    m_UV.push_back(glm::vec2(1, 1));
    m_UV.push_back(glm::vec2(0, 1));
    glm::vec2 AtlasSize=MainTextureAtlas.GetAtlasSize();
    if(AtlasSize.y<m_BaseSize.y) AtlasSize.y=m_BaseSize.y;
    MainTextureAtlas.ImageResizeCanvas(AtlasSize.x+m_BaseSize.x, AtlasSize.y);
    MainTextureAtlas.ImageAdd(*this);
}
/**
 * @brief Free memory from Sprite
 * 
 */
void Sprite::DeleteSprite(){
    if(m_hasTexture)
        delete m_Pixels;
}
/**
 * @brief Set poisition of the sprite
 * 
 * @param _pos position where the sprite should be rendered
 */
void Sprite::setPosition (glm::vec2 _pos) {m_Pos=_pos;}
/**
 * @brief Scales the sprite
 * 
 * @param _scale scale in vec2, where (1,1) is the base scale
 */
void Sprite::setScale(glm::vec2 _scale)  {m_Size=_scale*m_BaseSize;}
/**
 * @brief Set the color in which the sprite will be rendered
 * 
 * @param _color Color in RGBA format
 */
void Sprite::setColor(glm::vec4 _color)  {m_Color=_color;}
bool Sprite::operator==(const Sprite& spr2) const{
    return m_Pos==spr2.m_Pos && m_Size==spr2.m_Size
        && m_UV==spr2.m_UV && m_texSlot==spr2.m_texSlot;
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
TexturePage::TexturePage(int _width, int _height, int _channel_num, int _slot): m_xOffset(0),m_yOffset(0),m_Width(_width), m_Height(_height), m_ChannelNum(_channel_num), m_Slot(_slot){
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
    std::map<std::string, Sprite*>::iterator it;
    for (it=SpritesTotal.begin();it!=SpritesTotal.end();it++){
        Sprite* spr = it->second;
        float x1= 0,
			  y1= (float)spr->m_UV[0].y*m_Height/new_height,
			  x2= (float) spr->getBaseSize().x / new_width,
			  y2=y1+ (float) spr->getBaseSize().y / new_height;
        spr->m_UV[0]=glm::vec2(x1,y1);
        spr->m_UV[1]=glm::vec2(x2,y1);
        spr->m_UV[2]=glm::vec2(x2,y2);
        spr->m_UV[3]=glm::vec2(x1,y2);
    }
    m_ChannelNum = channel_num;
    m_Width = new_width;
    m_Height = new_height;
}
/**
 * @brief Add a sprite to the texture page
 * 
 * @param sprite The sprite object to be added to the texture page
 */
void TexturePage::ImageAdd(Sprite& sprite) {
	if(sprite.hasTexture()){
		unsigned char* data = sprite.m_Pixels;
		int width = sprite.m_BaseSize.x;
		int height = sprite.m_BaseSize.y;
        
        if(SpritesTotal.size()!=0)
            m_xOffset=width-width;
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