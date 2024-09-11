#include "ngestd.h"
std::vector<std::reference_wrapper<Sprite>> SpritesToRender;
std::wstring ar_fix(std::wstring str){
    for(size_t i = 0; i<str.size();i++){
        
        size_t pos = ar_has_mi.find(str[i]);
        if(pos!=std::wstring::npos) {
            str[i]=has_mi[pos];
            continue;
        }
        pos = ar_has_no_mi.find(str[i]);
        if(pos!=std::wstring::npos) str[i]=has_no_mi[pos];
    }
    std::wstring out = str;
    
    bool cp_has_mi=false;
    for(size_t i = 0; i<str.size();i++){
        wchar_t c = str[i];
        wchar_t cn = str[(i<str.size()) ? i+1:i];
        bool c_has_mi = (has_mi.find(c)!=std::wstring::npos) ? true:false;
        bool cn_is_alphabet = (ar_alphabet.find(cn)!=std::wstring::npos) ? true:false;
        if(c_has_mi){
            if(!cp_has_mi && cn_is_alphabet) c+=2;
            else if(cp_has_mi && cn_is_alphabet) c+=3;
            else if(cp_has_mi) c+=1;
            cp_has_mi=true;
            out[i]=c;
            continue;
        }
        bool c_has_no_mi = (has_no_mi.find(c)!=std::wstring::npos) ? true:false;
        if(cp_has_mi && c_has_no_mi) c++;
        cp_has_mi=false;
        out[i]=c;
    }
    std::wstring temp=out;
    for(size_t j=0;j<out.size();j++){
        out[j]=temp[out.size()-1-j];
    }
    return out;
}
/**
 * @brief Load an ngesprite image to memory
 * 
 * @param path path to ngesprite file
 * @param spr pointer to the object to be saved in
 * @return std::vector<unsigned char*> List of pixel data of the loaded sprites (for sprites that has multiple frames)
 */
std::vector<unsigned char*> LoadNGESprite(const char* path, Sprite* spr){
    std::ifstream in(path, std::ios::binary);
    
    in.seekg(0, std::ios::end);
    unsigned long long size=in.tellg();
    in.seekg(0, std::ios::beg);
    std::vector<char> t_Data;
    t_Data.resize(size);
    in.read(&t_Data[0], size);
    in.close();
    
    unsigned long long cursor =0;
    /*-------------Read the name of the sprite--------------*/
    unsigned t_NameLength = t_Data[cursor];
    cursor+=t_NameLength;
    char t_Name[t_NameLength+1];
    std::memcpy(t_Name, &t_Data[1], t_NameLength);
    t_Name[t_NameLength]='\0';
    spr->m_Name=t_Name;
    cursor++;
    /*--------------------Get frames count-------------------*/
    spr->m_Frames=t_Data[cursor];
    cursor++;
    /*------------------Load Image to memory-----------------*/
    unsigned char t_Size[8] {0};
    unsigned char t_Head[4];
    unsigned char t_Tail[4];

    unsigned i=0;
    std::vector<unsigned char*> t_Pixels;
    spr->m_BaseSize.clear();
    spr->m_UV.clear();
    while(i<spr->m_Frames){
        
        std::memcpy(t_Size, &t_Data[cursor], 8);
        cursor+=8;
        std::memcpy(t_Head, &t_Data[cursor], 3);
        t_Head[3]='\0';
        cursor+=3;
        if(strcmp((const char*)t_Head, "BEG")){
            std::cout << "Error loading frame! File is corrupted." << std::endl;
            for(unsigned char* p : t_Pixels){
                delete p;
            }
            t_Pixels.clear();
            return t_Pixels;
        }
        unsigned long long t_FrameSize=0;
        std::memcpy(&t_FrameSize, t_Size, sizeof(t_FrameSize));
        std::string t_Sprite(t_Data.begin()+cursor, t_Data.begin()+cursor+t_FrameSize);
        
        cursor+= t_FrameSize;
        std::memcpy(t_Tail, &t_Data[cursor], 3);
        cursor+=3;
        t_Tail[3]='\0';
        if(strcmp((const char*)t_Tail, "END")){
            std::cout << "Error loading frame! File is corrupted." << std::endl;
            for(unsigned char* p : t_Pixels){
                delete p;
            }
            t_Pixels.clear();
            return t_Pixels;
        }
        int _width, _height, _bpp;
        
        unsigned char* m_Pixels = stbi_load_from_memory((unsigned char*)t_Sprite.c_str(), t_Sprite.length(), &_width, &_height, &_bpp, 4);
        if(stbi_failure_reason()){
            std::cout << "Error loading frame! Image(s) in file are corrupted." << std::endl;
            for(unsigned char* p : t_Pixels){
                delete p;
            }
            t_Pixels.clear();
            return t_Pixels;
        }
        spr->m_Widest=(spr->m_Widest>(unsigned)_width)? spr->m_Widest:_width;
        spr->m_Heighest=(spr->m_Heighest>(unsigned)_height)? spr->m_Heighest:_height;
        spr->m_HeightCombined+=_height;
        spr->m_WidthCombined+=_width;
        spr->m_BaseSize.push_back(glm::vec2(_width, _height));
        t_Pixels.push_back(m_Pixels);
        std::vector<glm::vec2> UV;
        UV.push_back(glm::vec2(0, 0));
        UV.push_back(glm::vec2(1, 0));
        UV.push_back(glm::vec2(1, 1));
        UV.push_back(glm::vec2(0, 1));
        spr->m_UV.push_back(UV);
        i++;
    }
    
    spr->m_Size=glm::vec2(spr->m_Widest, spr->m_Heighest);
    spr->m_hasTexture=true;
    
    
    SpritesTotal.insert({spr->m_Name, spr});
   
    return t_Pixels;
}
/**
 * @brief Load nge sprites located in the 'Sprites' directory to memory for use\n
 * Sprites are then used by calling Object2D::SpriteSet(std::string)\n
 * Names of the sprites are the same as the nge files names (without the .ngesprite)
 */
void LoadSpritesToMemroy(){
    DIR *dr;
    dirent *en;
    dr = opendir("Sprites");
    std::regex ext(".*\\.ngesprite$");
    std::vector<std::vector<unsigned char*>> t_Pixels;
    glm::vec2 t_TextureAtlasSize(0);
    if (dr) {
        while ((en = readdir(dr)) != NULL) {
            std::string sname="Sprites/";
            sname+=en->d_name;
            if(std::regex_match(sname, ext)){
                Sprite *spr= new Sprite(glm::vec2(0), glm::vec2(1));
                
                t_Pixels.push_back(LoadNGESprite(sname.c_str(), spr));
                t_TextureAtlasSize.x=(t_TextureAtlasSize.x>spr->getSize().x)? t_TextureAtlasSize.x : spr->getSize().x;
                t_TextureAtlasSize.y+=spr->getSumSize().y;
                
            }
        }
        closedir(dr);
    }
    
    MainTextureAtlas.ImageResizeCanvas(t_TextureAtlasSize.x, t_TextureAtlasSize.y);
    std::map<std::string, Sprite*>::iterator sprIT = SpritesTotal.begin();
    for(std::vector<unsigned char*> pixels: t_Pixels){
        Sprite* spr = sprIT->second;
        int t_Frame=0;
        for(unsigned char* p: pixels){
            MainTextureAtlas.ImageAdd(p, spr, t_Frame);
            t_Frame++;
            delete p;
        }
        sprIT++;
        
    }
    
    
    
}
/**
 * @brief Draw a single point on screen
 * 
 * @param x coordinates on x-axis
 * @param y coordinates on y-axis
 */
void ngestd::DrawPoint(float x, float y){
    glBegin(GL_POINTS);
    glVertex2f(x,y);
    glEnd();
}
/**
 * @brief Draw a rectangle on screen
 * 
 * @param x1 coordinates of the top-left corner on the x-axis
 * @param y1 coordinates of the top-left corner on the y-axis
 * @param x2 coordinates of the bottom-right corner on the x-axis
 * @param y2 coordinates of the bottom-right corner on the y-axis
 * @param outline A flag to draw the rectangle as an outline (true) or as a filled rectangle(false)
 */
void ngestd::DrawRectangle(float x1, float y1, float x2, float y2, bool outline){
    glBegin(GL_QUADS);
    if(outline)
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1,y1);
    glVertex2f(x2,y1);
    glVertex2f(x2,y2);
    glVertex2f(x1,y2);
    glEnd();
}
/**
 * @brief Set the draw color for the engine
 * 
 * @param col color in vec3 (float values between 0 and 1)
 */
void ngestd::DrawSetColor(glm::vec3 col){
    glColor3f(col.x,col.y,col.z);
}
/**
 * @brief Get the current draw color of the engine
 * 
 * @return glm::vec3 float values between 0 and 1
 */
glm::vec3 ngestd::DrawGetColor(){
    float col[3];
    glGetFloatv(GL_CURRENT_COLOR,col);
    return glm::make_vec3(col);
}