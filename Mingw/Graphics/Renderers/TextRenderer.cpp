#include "TextRenderer.h"

TextRenderer::TextRenderer(std::string fnt) : fontName(fnt), indexCount(0), fontTexPage(2048,2048, 1), m_fence(nullptr){
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
    FT_Set_Pixel_Sizes(face, 0, 16);
    FT_ULong  charcode;
    FT_UInt   glyph_index;

    charcode = FT_Get_First_Char(face, &glyph_index);
    while (glyph_index != 0){
        if (FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER) == 0) {
            FT_GlyphSlot slot = face->glyph;
            FT_Bitmap& bmp = face->glyph->bitmap;
            Glyph g;
            g.width    = bmp.width;
            g.height   = bmp.rows;
            g.bearingX = slot->bitmap_left;
            g.bearingY = slot->bitmap_top;
            g.advance  = slot->advance.x >> 6;
            unsigned char* data = bmp.buffer;
            g.bitmap.assign(data, data + bmp.width * bmp.rows);
            g.texturePage = -1;
            g.uv = glm::vec2(0.0f);
            glyphs[charcode] = std::move(g);
            // bmp.buffer contains the grayscale bitmap
            // store it + metrics somewhere (texture atlas, map, etc.)
        }

        charcode = FT_Get_Next_Char(face, charcode, &glyph_index);
    }
    FT_Done_Face(face);
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
    glNamedBufferStorage(m_VBO, MAX_CHARS * 4 * sizeof(charVertex), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    glVertexArrayVertexBuffer(m_VAO,0,m_VBO,0, sizeof(charVertex));
    

    unsigned short indices[MAX_CHARS * 6];
    unsigned offset = 0;
    for(unsigned i=0;i<MAX_CHARS*6;i+=6){
        indices[ i ]=offset;
        indices[i+1]=offset+1;
        indices[i+2]=offset+2;
        indices[i+3]=offset+2;
        indices[i+4]=offset+3;
        indices[i+5]=offset;
        offset+=4;
    }
    

    glCreateBuffers(1, &m_IBO);
    glNamedBufferStorage(m_IBO, MAX_CHARS * 6 * sizeof(short), indices, 0);
    glVertexArrayElementBuffer(m_VAO, m_IBO);

    for(int i=0;i<4;i++){
        glEnableVertexArrayAttrib(m_VAO,i);
        glVertexArrayAttribBinding(m_VAO,i,0);
    }
    glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(charVertex, pos));
    glVertexArrayAttribFormat(m_VAO, 1, 2, GL_FLOAT, GL_FALSE, offsetof(charVertex, uv));
    glVertexArrayAttribFormat(m_VAO, 2, 4, GL_FLOAT, GL_FALSE, offsetof(charVertex, col));
    glVertexArrayAttribIFormat(m_VAO, 3, 1, GL_INT, offsetof(charVertex, texturePage));
    
    m_Shader = Engine::CreateShader(Engine::LoadShaderFromFile("./Graphics/Shaders/fontShader.vs").c_str(),
                                    Engine::LoadShaderFromFile("./Graphics/Shaders/fontShader.fs").c_str());

    vboBasePtr = glMapNamedBufferRange(m_VBO, 0, MAX_CHARS * 4 * sizeof(charVertex), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    charVertBuff = (charVertex*)vboBasePtr;
}
TextRenderer::Glyph& TextRenderer::loadChar(FT_ULong c){
    auto it = glyphs.find(c);
    
    if(it != glyphs.end()){
        Glyph& g = it->second;
        
        if(!g.bitmap.empty() && g.width > 0 && g.height > 0){
            
            g.uv = fontTexPage.maxRect(g.bitmap.data(), g.width, g.height);
            g.bitmap.clear();
            g.bitmap.shrink_to_fit();
            g.texturePage = fontTexPage.GetTextureSlot();
        }
    }
    else{
        std::cout << "Glyph not found" << std::endl;
        return glyphs.begin()->second;
    }
    return it->second;
}
void TextRenderer::renderText(std::wstring str, float x, float y, float z){
    float pen_x = x;
    glm::vec2 texSize = fontTexPage.GetAtlasSize();
    if(!indexCount && m_fence){
        glClientWaitSync(m_fence, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED);
        glDeleteSync(m_fence);
        m_fence=nullptr;
    }
    
    for(wchar_t c : str){
        
        Glyph& g = loadChar(c);
        
        //First Vertex
        charVertBuff->pos= glm::vec3(pen_x + g.bearingX, y - g.bearingY, z);
        charVertBuff->uv = g.uv;
        charVertBuff->col = glm::vec4(1.0f);
        charVertBuff->texturePage = g.texturePage;
        charVertBuff++;
        //Second Vertex
        charVertBuff->pos= glm::vec3(pen_x + g.bearingX + g.width, y - g.bearingY, z);
        charVertBuff->uv = g.uv + glm::vec2(g.width,0)/texSize;
        charVertBuff->col = glm::vec4(1.0f);
        charVertBuff->texturePage = g.texturePage;
        charVertBuff++;
        //Third Vertex
        charVertBuff->pos= glm::vec3(pen_x + g.bearingX + g.width, y - g.bearingY + g.height, z);
        charVertBuff->uv = g.uv + glm::vec2(g.width,g.height)/texSize;
        charVertBuff->col = glm::vec4(1.0f);
        charVertBuff->texturePage = g.texturePage;
        charVertBuff++;
        //Fourth Vertex
        charVertBuff->pos= glm::vec3(pen_x + g.bearingX, y - g.bearingY + g.height, z);
        charVertBuff->uv = g.uv + glm::vec2(0,g.height)/texSize;
        charVertBuff->col = glm::vec4(1.0f);
        charVertBuff->texturePage = g.texturePage;
        charVertBuff++;
        pen_x +=g.advance;
        indexCount+=6;
        
    }
}
void TextRenderer::flush(){
    glUseProgram(m_Shader);
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "proj_matrix"),1,GL_FALSE,Engine::getOrthoMatrix());
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "vw_matrix"),1,GL_FALSE,Engine::getViewMatrix2D());
    glUniform1i(glGetUniformLocation(m_Shader, "texture[0]"), fontTexPage.GetTextureSlot());
    fontTexPage.Bind();
    glBindVertexArray(m_VAO);
    //glDisable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_DEPTH_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, NULL);
    glDisable(GL_BLEND);
    //glDepthMask(GL_TRUE);
    //glEnable(GL_DEPTH_TEST);
    charVertBuff = (charVertex*)vboBasePtr;
    m_fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    glBindVertexArray(0);
    indexCount=0;
    fontTexPage.Unbind();

    glUseProgram(0);
}