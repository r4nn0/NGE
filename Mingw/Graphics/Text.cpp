#include "Text.h"

Text::Text(std::wstring _text, glm::vec3 _pos): text(_text), pos(_pos), col(glm::vec4(1.0)), scale(1.0), rot(0.0){
}
Text::~Text(){
    
}