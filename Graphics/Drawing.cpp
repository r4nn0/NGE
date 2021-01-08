/*#include "Drawing.h"


namespace Drawing{
    void draw_set_color(GLfloat[3]);
    void draw_set_alpha(GLfloat);
    void draw_line(GLfloat, GLfloat, GLfloat, GLfloat);
    void draw_circle(GLfloat, GLfloat, GLfloat, bool);
    void draw_rectangle(GLfloat, GLfloat, GLfloat, GLfloat, bool);
    void draw_text(int, int, auto);
    typedef struct Color{
        float r,g,b;
    public:
        Color(){r=0;g=0;b=0;}
        Color(int R, int G, int B) : r((float)R/255), g((float)G/255), b((float)B/255){}
        /*
        Color& operator=(Color& v){
            r=v.r;
            g=v.g;
            b=v.b;
            return *this;
        }
        Color& operator+=(const Color& v){
            r+=v.r;
            g+=v.g;
            b+=v.b;
            return *this;
        }
    }Color;
    void print(auto);

};


void Drawing::draw_set_color(GLfloat color[3]){
    glColor3fv(color);
}
void Drawing::draw_set_alpha(GLfloat alpha){
    GLfloat clr[4];
    glGetFloatv(GL_CURRENT_COLOR,clr);
    clr[3]=alpha;
    glColor4fv(clr);
}
void Drawing::draw_line(GLfloat x1, GLfloat y1,GLfloat x2,GLfloat y2){
    glLoadIdentity();
    GLfloat lv[]={x1,y1,x2,y2};
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2,GL_FLOAT,0,lv);
    glDrawArrays(GL_LINES,0,2);
    glDisableClientState(GL_VERTEX_ARRAY);
}
void Drawing::draw_circle(GLfloat x, GLfloat y, GLfloat r, bool outline){
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
void Drawing::draw_rectangle(GLfloat x1, GLfloat y1,GLfloat x2, GLfloat y2, bool outline){
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

void Drawing::draw_text(int x, int y, auto text){
    glLoadIdentity();
    glRasterPos2i(x, y);
    std::stringstream ss;
    ss<<text;
    for(const char* c=ss.str().c_str(); *c != 0; c++){
        glutBitmapCharacter((void*)(0x8) , *c);
    }
}
void Drawing::print(auto str){
    std::cout << str;
}
*/
