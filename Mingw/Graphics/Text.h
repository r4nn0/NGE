#ifndef TEXT_HEADER
#define TEXT_HEADER
#include <iostream>
#include <glm/glm.hpp>
class Text{
public:
    Text(std::wstring=L"", glm::vec3 = glm::vec3(0.0));
    ~Text();
    std::wstring text;
    glm::vec3 pos;
    glm::vec4 col;
    float scale;
    float rot;

};

#endif