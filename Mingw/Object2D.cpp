#include "Object2D.h"

Object2D::Object2D(const char* path) : position(glm::vec2(0)),
                                       scale(glm::vec2(1)),
                                       color(glm::vec4(1)),
                                       frame_index(0), anim_speed(0),
                                       sprite(path, glm::vec2(0)){
    frames.push_back(sprite);
}

Object2D::Object2D() : position(glm::vec2(0)),
                       scale(glm::vec2(1)),
                       color(glm::vec4(1)),
                       frame_index(0), anim_speed(0),
                       sprite(glm::vec2(0),glm::vec2(1)){
    frames.push_back(sprite);
}
Object2D::~Object2D(){
    sprite.DeleteSprite();
}
void Object2D::Update(){
    sprite.setPosition(position);
    sprite.setScale(scale);
    sprite.setColor(color);
    sprite=frames[frame_index].get();
}
void Object2D::Render(){
    bool found=false;
    for(Sprite &spr: SpritesToRender){
        if (sprite==spr) {
            found=true;
            break;
        }
    }
    if(!found)
        SpritesToRender.push_back(sprite);
}
/*
void Object2D::SpriteSet(const char* name){
    try{
        sprite=SpritesTotal.at(name);
    }catch(const std::out_of_range&){
        sprite=SpritesTotal.begin()->second;
    }
}*/