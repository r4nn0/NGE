#include "ngetype.h"

/// Constructors
ngetype::Color::Color(){r=0;g=0;b=0;}
ngetype::Color::Color(int R, int G, int B) : r((float)R/255), g((float)G/255), b((float)B/255){}
ngetype::Color::Color(float R, float G, float B) : r(R), g(G), b(B){}
/// Addition Assignment Operator
ngetype::Color& ngetype::Color::operator+=(const ngetype::Color& c2){
    this->r+=c2.r;
    this->g+=c2.g;
    this->b+=c2.b;
    return *this;
}


/// Constructors
ngetype::vec2::vec2(){x=0;y=0;}
ngetype::vec2::vec2(float x, float y) : x(x), y(y){}


/// Constructors
ngetype::string::string(){}
ngetype::string::string(const char* ca){
    strcpy(str,ca);
}
/// Addition Assignment Operator
ngetype::string& ngetype::string::operator+=(const ngetype::string& str2){
    strcat(this->str, str2.str);
    return *this;
}
ngetype::string& ngetype::string::operator+=(const char* str2){
    strcat(this->str,str2);
    return *this;
}
/// Addition Operator
ngetype::string& ngetype::string::operator+(const ngetype::string& str2){
    strcat(this->str,str2.str);
    return *this;
}
ngetype::string& ngetype::string::operator+(const char* str2){
    strcat(this->str,str2);
    return *this;
}
/// Assignment Operator
ngetype::string& ngetype::string::operator=(const ngetype::string& str2){
    strcpy(this->str,str2.str);
    return *this;
}
ngetype::string& ngetype::string::operator=(const char* str2){
    strcpy(this->str,str2);
    return *this;
}
/// Subscript Operator
char ngetype::string::operator[](const int index){
    return str[index];
}
/// Get String Length
int ngetype::string::length(){
    return ngetype::strlen(*this);
}
int ngetype::strlen(ngetype::string& str){
    int len=0;
    for(int i=0; str[len]!='\0';i++){
        len++;
    }
    return len;
}
std::ostream& ngetype::operator<<(std::ostream& out, const ngetype::string& str1){
    return out<< str1.str;
}
