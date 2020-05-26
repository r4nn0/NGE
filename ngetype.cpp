#include "ngetype.h"

///---------------------Color----------------------///
ngetype::Color::Color(){r=0;g=0;b=0;}
ngetype::Color::Color(int R, int G, int B) : r((float)R/255), g((float)G/255), b((float)B/255){}
ngetype::Color& ngetype::Color::operator+=(const ngetype::Color& c2){
    this->r+=c2.r;
    this->g+=c2.g;
    this->b+=c2.b;
    return *this;
}
///---------------------Pixel----------------------///
ngetype::Pixel::Pixel(){x=0;y=0;}
ngetype::Pixel::Pixel(float x, float y) : x(x), y(y){}

///----------------------String----------------------///
ngetype::string::string(){}
ngetype::string::string(char *ca){
    strcpy(str,ca);
}
ngetype::string& ngetype::string::operator+=(const ngetype::string& str2){
    strcat(this->str, str2.str);
    return *this;
}
ngetype::string& ngetype::string::operator+=(const char* str2){
    strcat(this->str,str2);
    return *this;
}
char* ngetype::string::operator+(const ngetype::string& str2){
    int length= this->length() + strlen(str2.str);
    char temp[length];
    strcpy(temp,this->str);
    strcat(temp,str2.str);
    return temp;
}
ngetype::string& ngetype::string::operator+(const char* str2){
    strcat(this->str,str2);
    return *this;
}
char ngetype::string::operator[](const int index){
    return str[index];
}
int ngetype::string::length(){
    return (int)strlen(this->str);
}
std::ostream& ngetype::operator<<(std::ostream& out, const ngetype::string& str1){
    return out<< str1.str;
}
