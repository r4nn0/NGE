#ifndef GAME_DRAWING
#define GAME_DRAWING


#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include FT_FREETYPE_H

#include "../gmath.h"
#include "../Engine.h"


namespace Drawing
{
    ngetype::Color draw_getpixel(int x, int y){
        ngetype::Color out(0,0,0);

        glReadPixels(x, y, 1,1, GL_RGB, GL_FLOAT, &out);
        return out;
    }
    /// Set Current Drawing Color ( Color(Object) )
    void draw_set_color(ngetype::Color c){
        GLfloat color[3];
        color[0]=c.r;
        color[1]=c.g;
        color[2]=c.b;
        glColor3fv(color);
    }
    /// Set Current Drawing Color ( Array of 3 floats (Red Green Blue) )
    void draw_set_color(GLfloat color[3]){
        glColor3fv(color);
    }
    /// Set Drawing Alpha between 0 and 1
    void draw_set_alpha(GLfloat alpha){
        GLfloat clr[4];
        glGetFloatv(GL_CURRENT_COLOR,clr);
        clr[3]=alpha;
        glColor4fv(clr);
    }
    /// Draw Line
    void draw_line(GLfloat x1, GLfloat y1,GLfloat x2,GLfloat y2){
        glLoadIdentity();
        GLfloat lv[]={x1,y1,x2,y2};
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2,GL_FLOAT,0,lv);
        glDrawArrays(GL_LINES,0,2);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    /// Draw Circle ( Center x and y, Radius, outline 0 or 1 where 0 draws filled circle and 1 draws outlined)
    void draw_circle(GLfloat x, GLfloat y, GLfloat r, bool outline){
        glLoadIdentity();
        glBegin(GL_LINE_LOOP);
        if (outline){
            for (float i=0; i <= 360; i+=.25f/(r/90)){
                glVertex2f(x+gmath::dcos(i)*r,y+gmath::dsin(i)*r);
            }
        }
        else {
            for (float i=0; i <= 360; i+=.25f/(r/90)){
                glVertex2f(x,y);
                glVertex2f(x+gmath::dcos(i)*r,y+gmath::dsin(i)*r);
            }
        }
        glEnd();
    }
    /// Draw Rectangle ( Top Left x and Y, Down Right X and Y, outline 1 or 0 where 0 draws a filled rectangle and 1 draws outlined)
    void draw_rectangle(GLfloat x1, GLfloat y1,GLfloat x2, GLfloat y2, bool outline){
        glLoadIdentity();
        if (outline){
            glBegin(GL_LINE_STRIP);
            glVertex2f(x1,y1);
            glVertex2f(x2,y1);
            glVertex2f(x2,y2);
            glVertex2f(x1,y2);
            glVertex2f(x1,y1);
            glEnd();
        }
        else{
            glBegin(GL_QUADS);
            glVertex2f(x1,y1);
            glVertex2f(x2,y1);
            glVertex2f(x2,y2);
            glVertex2f(x1,y2);
            glVertex2f(x1,y1);
            glEnd();
        }
    }
    /// Draw Text On OpenGL Window
    void draw_text(int x, int y, auto text){
        glLoadIdentity();
        glRasterPos2i(x, y);
        std::stringstream ss;
        ss<<text;
        for(const char* c=ss.str().c_str(); *c != 0; c++){
            glutBitmapCharacter((void*)(0x8) , *c);
        }
    }
    /// Print Text To The Console Screen
    void print(auto str){
        std::cout << str;
    }
    /// Set Blend Mode
    void draw_set_blend_mode(GLenum src, GLenum dest){
        glBlendFunc(src, dest);
    }
    /*ngetype::surface surface_create(int width, int height){
        unsigned int tex;
        glGenTextures(1, &tex);
        //glTexImage2D
    }*/

    inline int next_p2 (int a )
    {
        int rval=1;
        // rval<<=1 Is A Prettier Way Of Writing rval*=2;
        while(rval<a) rval<<=1;
        return rval;
    }

    FT_BitmapGlyph generateBitmapForFace(FT_Face face, int ch)
    {
        // Load the character glyph.
        if(FT_Load_Glyph( face, FT_Get_Char_Index( face, ch ), FT_LOAD_DEFAULT ))
            throw std::runtime_error("FT_Load_Glyph failed");

        // Move into a glyph object
        FT_Glyph glyph;
        if(FT_Get_Glyph( face->glyph, &glyph ))
            throw std::runtime_error("FT_Get_Glyph failed");

        // Convert to a bitmap
        FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
        return (FT_BitmapGlyph)glyph;
    }

    void storeTextureData(unsigned int const width,
                          unsigned int const height,
                          FT_Bitmap & bitmap,
                          std::vector<GLubyte> & expanded_data)
    {
        // Note: two channel bitmap (One for
        // channel luminosity and one for alpha).
        for(unsigned  int j = 0; j < height ; j++) {
            for(unsigned int i = 0; i < width; i++) {
                expanded_data[2 * (i + j * width)] = 255; // luminosity
                expanded_data[2 * (i + j * width) + 1] =
                (i >= bitmap.width || j >= bitmap.rows) ? 0 :
                bitmap.buffer[i + bitmap.width * j];
            }
        }
    }

    // Create A Display List Corresponding To The Given Character.
    void make_dlist ( FT_Face face, int ch, GLuint list_base, GLuint * tex_base ) {

        // Retrieve a bitmap for the given char glyph.
        FT_BitmapGlyph bitmap_glyph = generateBitmapForFace(face, ch);

        // This Reference Will Make Accessing The Bitmap Easier.
        FT_Bitmap& bitmap=bitmap_glyph->bitmap;

        // Get correct dimensions for bitmap
        unsigned int width = next_p2( bitmap.width );
        unsigned int height = next_p2( bitmap.rows ) ;

        // Use a vector to store texture data (better than a raw array).
        std::vector<GLubyte> expanded_data(2 * width * height, 0);

        // Does what it says
        storeTextureData(width, height, bitmap, expanded_data);

        // Texture parameters.
        glBindTexture( GL_TEXTURE_2D, tex_base[ch]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        // Create the texture Itself
        // GL_LUMINANCE_ALPHA to indicate 2 channel data.
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
            GL_LUMINANCE_ALPHA,  GL_UNSIGNED_BYTE, &expanded_data.front() );

        // Create the display list
        glNewList(list_base+ch,GL_COMPILE);

        glBindTexture(GL_TEXTURE_2D,tex_base[ch]);

        glPushMatrix();

        // Ensure right amount of space for each new char
        glTranslatef(bitmap_glyph->left,0,0);
        glTranslatef(0,bitmap_glyph->top-(bitmap.rows * 0.9),0);

        // Account for padding.
        float x = (float)bitmap.width / (float)width,
              y = (float)bitmap.rows / (float)height;
        glBegin(GL_QUADS);
        glTexCoord2d(0,0); glVertex2f(0,bitmap.rows);
        glTexCoord2d(0,y); glVertex2f(0,0);
        glTexCoord2d(x,y); glVertex2f(bitmap.width,0);
        glTexCoord2d(x,0); glVertex2f(bitmap.width,bitmap.rows);
        glEnd();
        glPopMatrix();
        glTranslatef(face->glyph->advance.x >> 6 ,0,0);
        glEndList();
    }

    inline void pushScreenCoordinateMatrix() {
        glPushAttrib(GL_TRANSFORM_BIT);
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(viewport[0],viewport[2],viewport[1],viewport[3],0,1);
        glPopAttrib();
    }
    inline void pop_projection_matrix() {
        glPushAttrib(GL_TRANSFORM_BIT);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glPopAttrib();
    }
    typedef struct Font{
        float h;
        std::vector<GLuint> textures;
        GLuint list_base;
        void init(const char * fname, unsigned int h) {
            textures.resize(1280);
            this->h=h;
            FT_Library library;
            if (FT_Init_FreeType( &library ))
                throw std::runtime_error("FT_Init_FreeType failed");
            FT_Face face;
            if (FT_New_Face( library, fname, 0, &face ))
                throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");
            FT_Set_Char_Size( face, h << 6, h << 6, 96, 96);
            list_base=glGenLists(1280);
            glGenTextures( 1280, &textures.front() );

            for(unsigned short int i=0;i<1280;i++) {
                make_dlist(face, i, list_base, &textures.front());
            }
            FT_Done_Face(face);
            FT_Done_FreeType(library);
        }

        void clean() {
            glDeleteLists(list_base,1280);
            glDeleteTextures(1280, &textures.front());
        }
    }Font;
    void print(const Font &ft_font, float x, float y, std::string const & text)  {
        glLoadIdentity();
        // We Want A Coordinate System Where Distance Is Measured In Window Pixels.
        pushScreenCoordinateMatrix();

        GLuint font=ft_font.list_base;
        // We Make The Height A Little Bigger.  There Will Be Some Space Between Lines.
        float h=ft_font.h/.63f;

        // Split text into lines
        std::stringstream ss(text);
        std::string to;
        std::vector<std::string> lines;
        while(std::getline(ss,to,'\n')){
            lines.push_back(to);
        }

        glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
        glMatrixMode(GL_MODELVIEW);
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glListBase(font);

        float modelview_matrix[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

        // This Is Where The Text Display Actually Happens.
        // For Each Line Of Text We Reset The Modelview Matrix
        // So That The Line's Text Will Start In The Correct Position.
        // Notice That We Need To Reset The Matrix, Rather Than Just Translating
        // Down By h. This Is Because When Each Character Is
        // Drawn It Modifies The Current Matrix So That The Next Character
        // Will Be Drawn Immediately After It.
        for(unsigned int i=0;i<lines.size();i++) {
            glPushMatrix();
            glLoadIdentity();
            glTranslatef(x,Engine::SCREEN_HEIGHT-y-h*i,0);
            glMultMatrixf(modelview_matrix);

            // The Commented Out Raster Position Stuff Can Be Useful If You Need To
            // Know The Length Of The Text That You Are Creating.
            // If You Decide To Use It Make Sure To Also Uncomment The glBitmap Command
            // In make_dlist().
            // glRasterPos2f(0,0);
            glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());
            // float rpos[4];
            // glGetFloatv(GL_CURRENT_RASTER_POSITION ,rpos);
            // float len=x-rpos[0]; (Assuming No Rotations Have Happend)
            glPopMatrix();
        }

        glPopAttrib();

        pop_projection_matrix();
    }

};


#endif // GAME_DRAWING
