#include "Object2D.h"
/**
 * @brief Construct a new Object2D
 * 
 * @param sprName the name of the sprite to use for the object
 */
Object2D::Object2D(std::string sprName) : position(glm::vec2(0)),
                                       scale(glm::vec2(1)),
                                       color(glm::vec4(1)),
                                       frame_index(0), anim_speed(0),
                                       sprite_name(sprName), bbox(1),sprite(nullptr){
    SpriteSet(sprite_name);
}
/**
 * @brief Default constructor for Object2D
 * 
 */
Object2D::Object2D() : position(glm::vec2(0)),
                       scale(glm::vec2(1)),
                       color(glm::vec4(1)),
                       frame_index(0), anim_speed(0),
                       sprite_name(""), bbox(1), sprite(nullptr){}
/**
 * @brief Destroy the Object2D
 * 
 */
Object2D::~Object2D(){
    // We don't want to delete the sprite because it might be in use by other objects
}
/**
 * @brief Update the object every frame
 * 
 */
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
/**
 * @brief Add the sprite of the object to the rendering list
 * 
 */
void Object2D::Render(){
    if(sprite==nullptr) return;
    for(Sprite &spr: SpritesToRender){
        if (*sprite==spr) {
            return;
        }
    }
    SpritesToRender.push_back(*sprite);
}
/**
 * @brief Set the sprite for the object
 * 
 * @param sprName name of the sprite to use (same name as the files in the Sprites folder without ".ngesprite")
 */
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