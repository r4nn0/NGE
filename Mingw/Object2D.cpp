#include "Object2D.h"

Object2D::Object2D(const char* path) : position(glm::vec2(0)),
                                       scale(glm::vec2(1)),
                                       color(glm::vec4(1)),
                                       frame_index(0), anim_speed(0),
                                       sprite(nullptr){
    std::map<std::string, Sprite*>::iterator pos = SpritesTotal.find(path);
    if(pos==SpritesTotal.end()){
        std::cout << "Sprite not found or not loaded to memory!" << std::endl;
        return;
    }
    sprite = pos->second;
}

Object2D::Object2D() : position(glm::vec2(0)),
                       scale(glm::vec2(1)),
                       color(glm::vec4(1)),
                       frame_index(0), anim_speed(0),
                       sprite(nullptr){}
Object2D::~Object2D(){
}
void Object2D::Update(){
    sprite->setPosition(position);
    sprite->setScale(scale);
    sprite->setColor(color);
    sprite->setFrameIndex(frame_index);
}
void Object2D::Render(){
    if(sprite==nullptr) return;
    for(Sprite &spr: SpritesToRender){
        if (*sprite==spr) {
            return;
        }
    }
    SpritesToRender.push_back(*sprite);
}
/*
void Object2D::SpriteSet(const char* name){
    try{
        sprite=SpritesTotal.at(name);
    }catch(const std::out_of_range&){
        sprite=SpritesTotal.begin()->second;
    }
}*/