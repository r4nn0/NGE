#include "Object2D.h"

Object2D::Object2D(const char* path) : position(glm::vec2(0)),
                                       scale(glm::vec2(1)),
                                       color(glm::vec4(1)),
                                       sprite(path, glm::vec2(0)){

}

Object2D::Object2D() : position(glm::vec2(0)),
                       scale(glm::vec2(1)),
                       color(glm::vec4(1)),
                       sprite(glm::vec2(0),glm::vec2(1)){
}
Object2D::~Object2D(){
    sprite.DeleteSprite();
}
void Object2D::Update(){
    sprite.setPosition(position);
    sprite.setScale(scale);
    sprite.setColor(color);
}
void Object2D::Render(){
    SpritesToRender.push_back(sprite);
}