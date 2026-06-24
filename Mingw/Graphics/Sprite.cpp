#include "Sprite.h"
std::map<std::string, Sprite*> SpritesTotal;
TexturePage MainTextureAtlas(4096,4096,4,0);

/**
 * @brief Create a sprite without texture
 * 
 * @param _pos position where to render the sprite
 * @param _size size of the sprite
 */
Sprite::Sprite(): m_Name(""),m_Frames(0), m_FrameIndex(0),m_Origin(glm::vec2(0,0)), m_hasTexture(false), m_texSlot(-1), m_Widest(0), m_Heighest(0),m_WidthCombined(0), m_HeightCombined(0), m_Scale(glm::vec2(1.0f)), m_Rotation(0.f), m_Position(glm::vec3(0.0f)){
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
void Sprite::setScale(glm::vec2 scale){
    m_Scale = scale;
}
void Sprite::setRotation(float rotation){
    m_Rotation = rotation;
}
void Sprite::setPosition(glm::vec3 pos){
    m_Position = pos;
}
/**
 * @brief Initialize the texture page where the sprites should be saved
 * 
 * @param slot texture slot to save in between 0 and 31 (default = 0)
 */
TexturePage::TexturePage(int slot) :m_Width(0), m_Height(0), m_ChannelNum(4), m_Slot(slot), m_Initiated(false){
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
TexturePage::TexturePage(int _width, int _height, int _channel_num, int _slot): m_Width(_width), m_Height(_height), m_ChannelNum(_channel_num), m_Slot(_slot), m_Initiated(false){
	m_TexturePage = (unsigned char*)malloc(m_Width*m_Height*m_ChannelNum);
    glGenTextures(1, &m_Texture);
    m_FreeRects.push_back({0,0,m_Width, m_Height});
}
bool TexturePage::Intersects(const Rect& a, const Rect& b){
    return !(
        a.x + a.width <= b.x ||
        b.x + b.width <= a.x ||
        a.y + a.height <= b.y ||
        b.y + b.height <= a.y
    );
}
bool TexturePage::Contains(const Rect& a, const Rect& b){
    return b.x>=a.x && b.y>=a.y && 
    b.x+b.width <= a.x+a.width  &&
    b.y+b.height <= a.y+a.height;
}
void TexturePage::SplitFreeRect(const Rect& freeRect, const Rect& usedRect){
    /// Top Rect
    if(usedRect.y > freeRect.y){
        newRects.push_back({
            freeRect.x, freeRect.y, freeRect.width, usedRect.y-freeRect.y
        });
    }
    /// Bot Rect
    if(usedRect.y + usedRect.height < freeRect.y + freeRect.height){
        newRects.push_back({
            freeRect.x, usedRect.y + usedRect.height, freeRect.width, (freeRect.y + freeRect.height) - (usedRect.y + usedRect.height)
        });
    }
    /// Left Rect
    if(usedRect.x > freeRect.x){
        newRects.push_back({
            freeRect.x, freeRect.y, usedRect.x - freeRect.x, freeRect.height
        });
    }
    /// Right Rect
    if(usedRect.x + usedRect.width < freeRect.x + freeRect.width){
        newRects.push_back({
            usedRect.x + usedRect.width, freeRect.y, (freeRect.x + freeRect.width) - (usedRect.x + usedRect.width), freeRect.height
        });
    }
}
void TexturePage::PruneFreeRects(){
    for(unsigned i=0; i<m_FreeRects.size(); i++){
        for(unsigned j=i + 1; j<m_FreeRects.size();){
            if(Contains(m_FreeRects[i], m_FreeRects[j])){
                m_FreeRects.erase(m_FreeRects.begin()+j);
            }else if(Contains(m_FreeRects[j], m_FreeRects[i])){
                m_FreeRects.erase(m_FreeRects.begin()+i);
                i--;
                break;
            }
            else j++;
        }
    }
}
void TexturePage::maxRect(unsigned char* data, Sprite* spr, int t_Frame){
    if(!m_Initiated){
        Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        Unbind();
        m_Initiated=true;
    }
    int width = spr->m_CurrentFrameSize[t_Frame].x;
	int height = spr->m_CurrentFrameSize[t_Frame].y;
    Rect* found = nullptr;
    int index = 0;
    for(Rect& rect: m_FreeRects){
        if(width <= rect.width && height<= rect.height){
            found = &rect;
            break;
        }
        index++;
    }
    if(!found) return;
    Rect newTex;
    newTex.x = found->x;
    newTex.y = found->y;
    newTex.width = width;
    newTex.height = height;
    for (unsigned i=0;i<m_FreeRects.size();){
        if(Intersects(m_FreeRects[i], newTex)){
            SplitFreeRect(m_FreeRects[i], newTex);
            m_FreeRects.erase(m_FreeRects.begin() + i);
        }else i++;
    }
    while(!newRects.empty()){
        m_FreeRects.push_back(newRects.back());
        newRects.pop_back();
    }
    PruneFreeRects();
    float x1 = (float)newTex.x / m_Width,
          y1 = (float)newTex.y/m_Height,
          x2 = (float)(newTex.x + width) / m_Width,
          y2 = (float)(newTex.y + height) / m_Height;

    spr->m_UV[t_Frame][0]=glm::vec2(x1,y1);
    spr->m_UV[t_Frame][1]=glm::vec2(x2,y1);
    spr->m_UV[t_Frame][2]=glm::vec2(x2,y2);
    spr->m_UV[t_Frame][3]=glm::vec2(x1,y2);
    //std::cout << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
    spr->m_texSlot=m_Slot;
    Bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, newTex.x, newTex.y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    Unbind();
    /*for (unsigned int y = 0;y < height;y++) {
        int srcIndex = width * y * m_ChannelNum;
        int dstIndex = (newTex.x + m_Width *(y+newTex.y)) * m_ChannelNum;
        memcpy(&m_TexturePage[dstIndex], &data[srcIndex], width*m_ChannelNum);
    }*/
}
glm::vec2 TexturePage::maxRect(unsigned char* data, int width, int height){
    if(!m_Initiated){
        Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        Unbind();
        m_Initiated=true;
    }
    Rect* found = nullptr;
    int index = 0;
    for(Rect& rect: m_FreeRects){
        if(width <= rect.width && height<= rect.height){
            found = &rect;
            break;
        }
        index++;
    }
    //if(!found) return;
    Rect newTex;
    newTex.x = found->x;
    newTex.y = found->y;
    newTex.width = width;
    newTex.height = height;
    for (unsigned i=0;i<m_FreeRects.size();){
        if(Intersects(m_FreeRects[i], newTex)){
            SplitFreeRect(m_FreeRects[i], newTex);
            m_FreeRects.erase(m_FreeRects.begin() + i);
        }else i++;
    }
    while(!newRects.empty()){
        m_FreeRects.push_back(newRects.back());
        newRects.pop_back();
    }
    PruneFreeRects();
    float x1 = (float)newTex.x / m_Width,
          y1 = (float)newTex.y/m_Height,
          x2 = (float)(newTex.x + width) / m_Width,
          y2 = (float)(newTex.y + height) / m_Height;
    Bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, newTex.x, newTex.y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    Unbind();
    /*
    std::vector<glm::vec2> UVs;
    UVs.push_back(glm::vec2(x1,y1));
    UVs.push_back(glm::vec2(x2,y1));
    UVs.push_back(glm::vec2(x2,y2));
    UVs.push_back(glm::vec2(x1,y2));
    */
    return glm::vec2(x1, y1);
    /*for (unsigned int y = 0;y < height;y++) {
        int srcIndex = width * y * m_ChannelNum;
        int dstIndex = (newTex.x + m_Width *(y+newTex.y)) * m_ChannelNum;
        memcpy(&m_TexturePage[dstIndex], &data[srcIndex], width*m_ChannelNum);
    }*/
}
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