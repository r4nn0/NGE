#ifndef GAME_DRAWING
#define GAME_DRAWING


#include "../gmath.h"
#include "../Engine.h"

namespace Drawing
{
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
};


#endif // GAME_DRAWING
