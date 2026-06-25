#include "ngestd.h"
std::vector<std::reference_wrapper<Object3D>> Objects3DToRender;
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
    cursor++;
    /*--------------------Get frames count-------------------*/
    unsigned FrameCount=t_Data[cursor];
    cursor++;
    /*------------------Load Image to memory-----------------*/
    unsigned char t_Size[8] {0};
    unsigned char t_Head[4];
    unsigned char t_Tail[4];

    unsigned i=0;
    std::vector<unsigned char*> t_Pixels;
    spr->m_FramesSize.clear();
    spr->m_UVs.clear();
    while(i<FrameCount){
        
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
            std::cout << stbi_failure_reason() << std::endl;
            for(unsigned char* p : t_Pixels){
                delete p;
            }
            t_Pixels.clear();
            return t_Pixels;
        }
        if(spr->collisionMask.size()<=0){
            spr->collisionMask = std::vector<std::vector<bool>>(_height, std::vector<bool>(_width, false));
            for (int y = 0; y < _height; y++){
                for (int x = 0; x < _width; x++){
                    int index = (y * _width + x) * 4;
                    unsigned char a = m_Pixels[index + 3];
                    if (a >= 50){
                        spr->collisionMask[y][x] = true;
                    }
                }
            }
        }
        spr->m_FramesSize.push_back(glm::vec2(_width, _height));
        t_Pixels.push_back(m_Pixels);
        std::vector<glm::vec2> UV;
        UV.push_back(glm::vec2(0, 0));
        UV.push_back(glm::vec2(1, 0));
        UV.push_back(glm::vec2(1, 1));
        UV.push_back(glm::vec2(0, 1));
        spr->m_UVs.push_back(UV);
        i++;
    }
    
    SpritesTotal.insert({t_Name, spr});
   
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
    
    if (dr) {
        while ((en = readdir(dr)) != NULL) {
            std::string sname="Sprites/";
            sname+=en->d_name;
            if(std::regex_match(sname, ext)){
                Sprite *spr= new Sprite();
                t_Pixels.push_back(LoadNGESprite(sname.c_str(), spr));
            }
        }
        closedir(dr);
    }
    
    
    std::map<std::string, Sprite*>::iterator sprIT = SpritesTotal.begin();
    
    for(std::vector<unsigned char*> pixels: t_Pixels){
        Sprite* spr = sprIT->second;
        int t_Frame=0;
        for(unsigned char* p: pixels){
            MainTextureAtlas.maxRect(p, spr, t_Frame);
            t_Frame++;
            delete p;
        }
        sprIT++;
        
    }
}
void LoadModelsToMemory(){
    DIR *dr;
    dirent *en;
    dr = opendir("3DObjects");
    std::regex ext(".*\\.gltf$");
    
    if (dr) {
        while ((en = readdir(dr)) != NULL) {
            std::string objPath="3DObjects/";
            objPath+=en->d_name;
            if(std::regex_match(objPath, ext)){
                Object3D *obj = new Object3D(objPath.c_str());
                std::string objFullname = en->d_name;
                std::string objName = objFullname.substr(0, objFullname.find("."));
                //std::cout << objName << std::endl;
                ObjectsTotal.insert({objName, obj});
                
            }
        }
        closedir(dr);
    }
}
unsigned int CompileShader(unsigned int type, const char* source){
    unsigned int id = glCreateShader(type);

    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS,&result);
    if(!result){
        int length;
        glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length);
        //char *errMessage=(char*)malloc(length*sizeof(char));
        char errMessage[length];
        glGetShaderInfoLog(id, length,&length, errMessage);
        std::cout << "Couldn't Load " << (type==GL_VERTEX_SHADER?"Vertex":"Fragment")<<"Shader"<< std::endl;
        std::cout << errMessage << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

unsigned int CreateShader(const char*vertexShader, const char*fragmentShader){

    unsigned int program=glCreateProgram();

    unsigned int vs=CompileShader(GL_VERTEX_SHADER,vertexShader);
    unsigned int fs=CompileShader(GL_FRAGMENT_SHADER,fragmentShader);
    glAttachShader(program,vs);
    glAttachShader(program,fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}
std::string LoadShaderFromFile(const char* ShaderPath){
    FILE* file=fopen(ShaderPath,"rt");
    fseek(file,0,SEEK_END);
    unsigned long length=ftell(file);
    char* data=new char[length+1];
    memset(data,0,length+1);
    fseek(file,0,SEEK_SET);
    fread(data,1,length,file);
    fclose(file);
    std::string src(data);
    delete [] data;
    return src;
}

namespace Collision{
    Object2D* line(float x1, float y1, float x2, float y2, Object2D& other){
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);

        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;

        int err = dx - dy;

        
        glm::vec2 objSize = glm::vec2(other.collisionMask[0].size(), other.collisionMask.size());
        while(true){
            int localX = (int)((x1 - other.position.x)/other.scale.x);
            int localY = (int)((y1 - other.position.y)/other.scale.y);
            if(localX >= 0 && localX < objSize.x && localY>=0 && localY<objSize.y){
                if(other.collisionMask[localY][localX]){
                    return &other;
                }
            }

            if(x1==x2 && y1==y2) break;
            int e2 = err * 2;
            if(e2 > -dy){
                err -=dy;
                x1 += sx;
            }
            if(e2<dx){
                err +=dx;
                y1 +=sy;
            }
        }
        return nullptr;
    }
    Object2D* circle(float cx, float cy, float radius, Object2D& other){
        float minX = cx - radius;
        float maxX = cx + radius;
        float minY = cy - radius;
        float maxY = cy + radius;

        glm::vec2 objSize = glm::vec2(other.collisionMask[0].size(), other.collisionMask.size());
        for (float y = minY; y <= maxY; y++)
        {
            for (float x = minX; x <= maxX; x++)
            {
                float dx = x - cx;
                float dy = y - cy;

                if (dx * dx + dy * dy > radius * radius)
                    continue;

                int localX = (int)((x - other.position.x)/other.scale.x);
                int localY = (int)((y - other.position.y)/other.scale.y);

                if (localX < 0 || localY < 0 ||
                    localX >= objSize.x || localY >= objSize.y)
                    continue;

                if (other.collisionMask[localY][localX])
                    return &other;
            }
        }
        return nullptr;
    }
    Object2D* point(float x, float y, Object2D& other){
        glm::vec2 objSize = glm::vec2(other.collisionMask[0].size(), other.collisionMask.size());

        int localX = (int)((x - other.position.x)/other.scale.x);
        int localY = (int)((y - other.position.y)/other.scale.y);

        if (localX < 0 || localY < 0 || localX >= objSize.x || localY >= objSize.y)
            return nullptr;

        return (other.collisionMask[localY][localX]) ? &other : nullptr;
    }
}
