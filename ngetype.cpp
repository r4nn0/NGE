#include "ngetype.h"

//Color
ngetype::Color::Color(){r=0;g=0;b=0;}
ngetype::Color::Color(int R, int G, int B) : r((float)R/255), g((float)G/255), b((float)B/255){}
ngetype::Color& operator+=(ngetype::Color& c1,const ngetype::Color& c2){
    c1.r+=c2.r;
    c1.g+=c2.g;
    c1.b+=c2.b;
    return c1;
}
ngetype::Pixel::Pixel(){x=0;y=0;}
ngetype::Pixel::Pixel(float x, float y) : x(x), y(y){}
//Pixel
