#ifndef NGETYPE_H
#define NGETYPE_H


namespace ngetype
{

    typedef struct Color{
    public:
        float r,g,b;
        Color();
        Color(int, int, int);
    }Color;

    typedef struct Pixel{
    public:
        float x,y;
        Pixel();
        Pixel(float,float);
    }Pixel;
};

#endif // NGETYPE_H
