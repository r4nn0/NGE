#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H
#include <iostream>
#include <Engine.h>
#include <tesselator.h>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H
enum Align{
    LEFT = -1, TOP = -1, MIDDLE = 0, CENTER=0, RIGHT=1, BOTTOM=1
};
class TextRenderer{
private:
    struct GlyphMesh {
        std::vector<glm::vec3> vertices;
        std::vector<unsigned> indices;
        float advance;
        glm::vec2 bearing;
        glm::vec2 min;
        glm::vec2 max;
    };
    struct Glyph{
        glm::vec3 lpos;
        glm::vec3 wpos;
        glm::vec4 col;
        float scale;
        float rot;
    };
public:
    TextRenderer(std::string = "", float=1.0f);
    ~TextRenderer();
    void renderText(std::wstring, float, float, float);
    void setFontSize(float);
    void setFontColor(glm::vec4);
    void setFontRotation(float);
    void setValign(Align);
    void setHalign(Align);
    void flush();
private:

    void tessellateQuadratic(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2,
                         std::vector<glm::vec2>& vertices, float tolerance);
    void tessellateCubic(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3,
                     std::vector<glm::vec2>& vertices, float tolerance);
    GlyphMesh tessellateGlyph(FT_Face face, FT_UInt glyphIndex);
    GlyphMesh& loadGlyph(FT_ULong);
    void initRenderer();


    const unsigned MAX_VERTICES = 1000000;
    std::string fontName;
    glm::vec4 fontColor;
    float fontScale;
    float fontRotation;
    Align Valign;
    Align Halign;
    unsigned m_VAO, m_VBO, m_IBO, m_Shader;
    unsigned indexCount;
    unsigned vboOffset;
    
    void* vboBasePtr;
    void* m_indexBase;
    Glyph* charVertBuff;
    unsigned* indexBuff;
    GLsync m_fence;
    FT_Face face;
    std::unordered_map<FT_ULong, GlyphMesh> glyphCache;
    
};

#endif