#ifndef GAME_DRAWING
#define GAME_DRAWING


#include "../gmath.h"
#include "../Engine.h"

namespace Drawing
{
    void draw_set_color(GLfloat color[3]){
        glColor3fv(color);
    }
    void draw_set_alpha(GLfloat alpha){
        GLfloat clr[4];
        glGetFloatv(GL_CURRENT_COLOR,clr);
        clr[3]=alpha;
        glColor4fv(clr);
    }
    void draw_line(GLfloat x1, GLfloat y1,GLfloat x2,GLfloat y2){
        glLoadIdentity();
        GLfloat lv[]={x1,y1,x2,y2};
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2,GL_FLOAT,0,lv);
        glDrawArrays(GL_LINES,0,2);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
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

    void draw_text(int x, int y, auto text){
        glLoadIdentity();
        glRasterPos2i(x, y);
        std::stringstream ss;
        ss<<text;
        for(const char* c=ss.str().c_str(); *c != 0; c++){
            glutBitmapCharacter((void*)(0x8) , *c);
        }
    }
    void print(auto str){
        std::cout << str;
    }
    void draw_set_color(ngetype::Color c){
        GLfloat color[3];
        color[0]=c.r;
        color[1]=c.g;
        color[2]=c.b;
        glColor3fv(color);
    }
    void draw_set_blend_mode(GLenum src, GLenum dest){
        glBlendFunc(src, dest);
    }
};


#endif // GAME_DRAWING
