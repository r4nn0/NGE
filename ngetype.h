#ifndef NGETYPE_H
#define NGETYPE_H
#include <string.h>
#include <iostream>
namespace ngetype
{
    typedef struct Color{
    public:
        float r,g,b;
        Color();
        Color(int, int, int);
        Color(float, float, float);
        Color& operator+=(const Color&);
    }Color;
    typedef struct vec2{
    public:
        float x,y;
        vec2();
        vec2(float,float);
    }vec2;
    typedef struct string{
        char str[0];
    public:
        string();
        string(const char*);
        char operator[](const int);
        int length();

        string& operator+=(const string&);
        string& operator+=(const char*);

        string& operator=(const string&);
        string& operator=(const char*);

        string& operator+(const string&);
        string& operator+(const char*);
        /*friend std::ostream& operator<<(std::ostream& out,string& str){
            out<<str.str;
            return out;
        }*/
    }string;
    std::ostream& operator<<(std::ostream&, const ngetype::string&);
    int strlen(string&);
};

#endif // NGETYPE_H
