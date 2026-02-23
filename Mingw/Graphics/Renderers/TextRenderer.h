#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H
#include <iostream>
//#include <tesselator.h>
#include <unordered_map>
#include "../Sprite.h"
#include <ft2build.h>
#include FT_FREETYPE_H

class TextRenderer{
private:
    struct charVertex{
        glm::vec3 pos;
        glm::vec2 uv;
        glm::vec4 col;
        int texturePage;
    };
    struct Glyph {
        int texturePage;
        glm::vec2 uv;
        int width, height;
        int bearingX, bearingY;
        int advance;
        
        std::vector<unsigned char> bitmap;
    };
public:
    TextRenderer(std::string = "");
    
    ~TextRenderer();
    void renderText(std::wstring, float, float, float=0);
    void flush();
    std::unordered_map<FT_ULong, Glyph> glyphs;
private:
    void initRenderer();
    const unsigned MAX_CHARS = 10000;
    std::string fontName;
    unsigned m_VAO, m_VBO, m_IBO, m_Shader;
    unsigned indexCount;
    TexturePage fontTexPage;
    void* vboBasePtr;
    charVertex* charVertBuff;
    GLsync m_fence;
    Glyph& loadChar(FT_ULong);
};

#endif