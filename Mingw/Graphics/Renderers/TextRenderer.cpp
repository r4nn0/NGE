#include "TextRenderer.h"
void TextRenderer::tessellateQuadratic(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2,
                         std::vector<glm::vec2>& vertices, float tolerance) {
    auto subdivideQuad = [&](auto& self, glm::vec2 a, glm::vec2 b, glm::vec2 c) -> void {
        glm::vec2 ac = c - a;
        glm::vec2 ab = b - a;
        float d = glm::length(glm::cross(glm::vec3(ac, 0), glm::vec3(ab, 0)));
        
        if (d < tolerance) {
            vertices.push_back(a);
        } else {
            glm::vec2 m01 = (a + b) * 0.5f;
            glm::vec2 m12 = (b + c) * 0.5f;
            glm::vec2 m = (m01 + m12) * 0.5f;
            
            self(self, a, m01, m);
            self(self, m, m12, c);
        }
    };
    
    subdivideQuad(subdivideQuad, p0, p1, p2);
    vertices.push_back(p2);
}

void TextRenderer::tessellateCubic(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3,
                     std::vector<glm::vec2>& vertices, float tolerance) {
    auto subdivideCubic = [&](auto& self, glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d) -> void {
        // Check if cubic is flat enough
        glm::vec2 ad = d - a;
        float err = glm::length(glm::cross(glm::vec3(b - a, 0), glm::vec3(ad, 0))) +
                    glm::length(glm::cross(glm::vec3(c - a, 0), glm::vec3(ad, 0)));
        
        if (err < tolerance) {
            vertices.push_back(a);
        } else {
            glm::vec2 m01 = (a + b) * 0.5f;
            glm::vec2 m12 = (b + c) * 0.5f;
            glm::vec2 m23 = (c + d) * 0.5f;
            glm::vec2 m012 = (m01 + m12) * 0.5f;
            glm::vec2 m123 = (m12 + m23) * 0.5f;
            glm::vec2 m = (m012 + m123) * 0.5f;
            
            self(self, a, m01, m012, m);
            self(self, m, m123, m23, d);
        }
    };
    
    subdivideCubic(subdivideCubic, p0, p1, p2, p3);
    vertices.push_back(p3);
}

// Use libtess2 to triangulate the filled polygon
TextRenderer::GlyphMesh TextRenderer::tessellateGlyph(FT_Face face, FT_UInt glyphIndex) {
    GlyphMesh mesh;
    
    if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_NO_BITMAP | FT_LOAD_NO_HINTING)) {
        return mesh;
    }
    
    FT_GlyphSlot slot = face->glyph;
    if (slot->format != FT_GLYPH_FORMAT_OUTLINE) {
        return mesh;
    }
    
    mesh.advance = slot->advance.x / 64.0f;
    mesh.bearing = glm::vec2(slot->bitmap_left, slot->bitmap_top);
    
    TESStesselator* tess = tessNewTess(nullptr);
    
    // Feed contours directly from FT outline
    for (int i = 0; i < slot->outline.n_contours; i++) {
        int start = (i == 0) ? 0 : slot->outline.contours[i-1] + 1;
        int end = slot->outline.contours[i];
        int count = end - start + 1;
        
        std::vector<float> contour_verts;
        for (int j = start; j <= end; j++) {
            contour_verts.push_back(slot->outline.points[j].x / 64.0f);
            contour_verts.push_back(slot->outline.points[j].y / 64.0f);
            contour_verts.push_back(0.0f);
        }
        
        tessAddContour(tess, 3, contour_verts.data(), sizeof(float) * 3, count);
    }
    
    tessTesselate(tess, TESS_WINDING_NONZERO, TESS_POLYGONS, 3, 3, 0);
    
    const float* verts = tessGetVertices(tess);
    const int* elems = tessGetElements(tess);
    int nverts = tessGetVertexCount(tess);
    int nelems = tessGetElementCount(tess);
    
    mesh.vertices.resize(nverts);
    mesh.min = {+FLT_MAX, +FLT_MAX};
    mesh.max = {-FLT_MAX, -FLT_MAX};
    
    for (int i = 0; i < nverts; i++) {
        glm::vec3 vert = glm::vec3(verts[i*3], verts[i*3+1], verts[i*3+2]);
        mesh.vertices[i] = vert;
        mesh.min.x = std::min(mesh.min.x, vert.x);
        mesh.min.y = std::min(mesh.min.y, vert.y);

        mesh.max.x = std::max(mesh.max.x, vert.x);
        mesh.max.y = std::max(mesh.max.y, vert.y);
    }
    mesh.indices.resize(nelems * 3);
    for (int i = 0; i < nelems * 3; i++) {
        mesh.indices[i] = elems[i];
    }
    
    tessDeleteTess(tess);
    return mesh;
}
TextRenderer::TextRenderer(std::string fnt, float fntSize) : fontName(fnt), fontColor(glm::vec4(1.0f)), fontScale(fntSize), fontRotation(0.0f),
                                                             Valign(Align::TOP), Halign(Align::LEFT),
                                                             indexCount(0), vboOffset(0), m_fence(nullptr){
    FT_Library ft;
    if (FT_Init_FreeType(&ft)){
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }

    if(fontName=="")
        fontName = "C:/Windows/Fonts/arial.ttf";
    if (FT_New_Face(ft, fontName.c_str(), 0, &face)){
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
        return;
    }
    //FT_Set_Pixel_Sizes(face, 0, fntSize);
    FT_ULong  charcode;
    FT_UInt   glyph_index;

    charcode = FT_Get_First_Char(face, &glyph_index);
    while (glyph_index != 0){
        glyphCache[charcode] = tessellateGlyph(face, glyph_index);
        charcode = FT_Get_Next_Char(face, charcode, &glyph_index);
    }
    
    
    FT_Done_FreeType(ft);
    initRenderer();
}
TextRenderer::~TextRenderer(){
    glDeleteBuffers(1,&m_VBO);
    glDeleteBuffers(1,&m_IBO);
    glDeleteProgram(m_Shader);
}
void TextRenderer::initRenderer(){
    
    glCreateVertexArrays(1, &m_VAO);
    glCreateBuffers(1, &m_VBO);
    glNamedBufferStorage(m_VBO, MAX_VERTICES * sizeof(Glyph), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    glVertexArrayVertexBuffer(m_VAO,0,m_VBO,0, sizeof(Glyph));
    
    

    glCreateBuffers(1, &m_IBO);
    glNamedBufferStorage(m_IBO, MAX_VERTICES * sizeof(Glyph), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT );
    glVertexArrayElementBuffer(m_VAO, m_IBO);

    for(int i=0;i<5;i++){
        glEnableVertexArrayAttrib(m_VAO,i);
        glVertexArrayAttribBinding(m_VAO,i,0);
    }
    glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Glyph, lpos));
    glVertexArrayAttribFormat(m_VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Glyph, wpos));
    glVertexArrayAttribFormat(m_VAO, 2, 4, GL_FLOAT, GL_FALSE, offsetof(Glyph, col));
    glVertexArrayAttribFormat(m_VAO, 3, 1, GL_FLOAT, GL_FALSE, offsetof(Glyph, scale));
    glVertexArrayAttribFormat(m_VAO, 4, 1, GL_FLOAT, GL_FALSE, offsetof(Glyph, rot));
    
    m_Shader = Engine::CreateShader(Engine::LoadShaderFromFile("./Graphics/Shaders/fontShader.vs").c_str(),
                                    Engine::LoadShaderFromFile("./Graphics/Shaders/fontShader.fs").c_str());

    vboBasePtr = glMapNamedBufferRange(m_VBO, 0, MAX_VERTICES * sizeof(Glyph), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT );
    m_indexBase = glMapNamedBufferRange(m_IBO, 0, MAX_VERTICES * sizeof(Glyph), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT );
    indexBuff = (unsigned*) m_indexBase;
    charVertBuff = (Glyph*)vboBasePtr;
}
TextRenderer::GlyphMesh& TextRenderer::loadGlyph(FT_ULong charcode){
    if (glyphCache.find(charcode) == glyphCache.end()) {
        FT_UInt glyph_idx = FT_Get_Char_Index(face, charcode);
        glyphCache[charcode] = tessellateGlyph(face, glyph_idx);
    }
    return glyphCache[charcode];
}
void TextRenderer::setFontColor(glm::vec4 fntColor){
    fontColor = fntColor;
}
void TextRenderer::setFontSize(float fntSize){
    fontScale = fntSize;
}
void TextRenderer::setFontRotation(float fntRotation){
    fontRotation = fntRotation;
}
void TextRenderer::setValign(Align alignment){
    Valign = alignment;
}

void TextRenderer::setHalign(Align alignment){
    Halign = alignment;
}
void TextRenderer::renderText(std::wstring str, float x, float y, float z){
    float pen_x = 0;
    float minX = +FLT_MAX;
    float maxX = -FLT_MAX;
    float minY = +FLT_MAX;
    float maxY = -FLT_MAX;
    float xalign = 0;
    float yalign = 0;
    if(!indexCount && m_fence){
        glClientWaitSync(m_fence, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED);
        glDeleteSync(m_fence);
        m_fence=nullptr;
    }
    for(wchar_t c : str){
        GlyphMesh& g = loadGlyph(c);
        float gxMin = pen_x + g.min.x;
        float gxMax = pen_x + g.max.x;
        minX = std::min(minX, gxMin);
        maxX = std::max(maxX, gxMax);
        pen_x += g.advance;

        float gyMin = g.min.y;
        float gyMax = g.max.y;
        minY = std::min(minY, gyMin);
        maxY = std::max(maxY, gyMax);
    }
    xalign = (minX+maxX)/2 + (minX+maxX)*(float)Halign/2;
    yalign = (minY+maxY)/2 + (minY+maxY)*(float)Valign/2;
    pen_x=0;
    for(wchar_t c : str){
        GlyphMesh& g = loadGlyph(c);
        for(unsigned index : g.indices){
            indexBuff[indexCount++] = index + vboOffset;
        }
        for(glm::vec3 pos : g.vertices){
            charVertBuff[vboOffset].lpos = pos;
            charVertBuff[vboOffset].wpos = glm::vec3(x - xalign * fontScale + pen_x, y - yalign * fontScale, z * fontScale);
            charVertBuff[vboOffset].col = fontColor;
            charVertBuff[vboOffset].scale = fontScale;
            charVertBuff[vboOffset].rot = fontRotation;
            vboOffset++;
        }
        pen_x += g.advance * fontScale;
    }
}
void TextRenderer::flush(){
    glUseProgram(m_Shader);
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "proj_matrix"),1,GL_FALSE,Engine::getOrthoMatrix());
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "vw_matrix"),1,GL_FALSE,Engine::getViewMatrix2D());
    
    
    glBindVertexArray(m_VAO);
    //glDisable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_DEPTH_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL);
    glDisable(GL_BLEND);
    //glDepthMask(GL_TRUE);
    //glEnable(GL_DEPTH_TEST);
    
    m_fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    glBindVertexArray(0);
    indexCount=0;
    vboOffset=0;

    glUseProgram(0);
}