#include "Object2D.h"

Object2D::Object2D(std::string sprName) : position(glm::vec2(0)),
                                       scale(glm::vec2(1)),
                                       color(glm::vec4(1)),
                                       frame_index(0), anim_speed(0),
                                       sprite_name(sprName), bbox(1),sprite(nullptr){
    SpriteSet(sprite_name);
}

Object2D::Object2D() : position(glm::vec2(0)),
                       scale(glm::vec2(1)),
                       color(glm::vec4(1)),
                       frame_index(0), anim_speed(0),
                       sprite_name(""), bbox(1), sprite(nullptr){}
Object2D::~Object2D(){
}
void Object2D::Update(){
    bbox.left=position.x;
    bbox.top = position.y;
    bbox.right=position.x;
    bbox.bottom=position.y;
    if(sprite!=nullptr){
        
        sprite->setFrameIndex(frame_index);
        sprite->setPosition(position);
        sprite->setScale(scale);
        sprite->setColor(color);
        bbox.right+=sprite->getSize().x;
        bbox.bottom+=sprite->getSize().y;
    }
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
void Object2D::SpriteSet(std::string sprName){
    sprite_name=sprName;
    
    std::map<std::string, Sprite*>::iterator pos = SpritesTotal.find(sprite_name);
    if(pos==SpritesTotal.end()){
        std::cout << "Sprite not found or not loaded to memory!" << std::endl;
        return;
    }
    sprite = pos->second;
}
/*
void Object2D::SpriteSet(const char* name){
    try{
        sprite=SpritesTotal.at(name);
    }catch(const std::out_of_range&){
        sprite=SpritesTotal.begin()->second;
    }
}*/