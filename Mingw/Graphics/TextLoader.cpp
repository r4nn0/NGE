#include "TextLoader.h"

TextLoader::TextLoader(std::string fnt) : fontName(fnt){
    FT_Library ft;
    if (FT_Init_FreeType(&ft)){
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }

    if(fontName=="")
        fontName = "C:/Windows/Fonts/arial.ttf";
    FT_Face face;
    if (FT_New_Face(ft, fontName.c_str(), 0, &face)){
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
        return;
    }

}