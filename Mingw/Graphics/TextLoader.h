#ifndef TEXT_LOADER
#define TEXT_LOADER
#include <iostream>
#include <tesselator.h>
#include <ft2build.h>
#include FT_FREETYPE_H
class TextLoader{
public:
    TextLoader(std::string = "");
    ~TextLoader();
private:
    std::string fontName;
};

#endif