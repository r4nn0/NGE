#include "Object2D.h"
/**
 * @brief Construct a new Object2D
 * 
 * @param sprName the name of the sprite to use for the object
 */
Object2D::Object2D(std::string sprName) : position(glm::vec3(0)),
                                       scale(glm::vec2(1)),
                                       color(glm::vec4(1)),
                                       frame_index(0), anim_speed(0),
                                       sprite_name(sprName), bbox(1),sprite(nullptr){
    SpriteSet(sprite_name);
}
/**
 * @brief Destroy the Object2D
 * 
 */
Object2D::~Object2D(){
    delete sprite;
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
    
    SpritesToRender.push_back(*sprite);
}
/**
 * @brief Set the sprite for the object
 * 
 * @param sprName name of the sprite to use (same name as the files in the Sprites folder without ".ngesprite")
 */
void Object2D::SpriteSet(std::string sprName){
    sprite_name=sprName;
    if(sprite!=nullptr) delete sprite;
    sprite = new Sprite(glm::vec3(0.0), glm::vec2(32.0f));
    
    std::map<std::string, Sprite*>::iterator pos = SpritesTotal.find(sprite_name);
    if(pos==SpritesTotal.end()){
        std::cout << "Sprite not found or not loaded to memory!" << std::endl;
        return;
    }
    
    *sprite = *(pos->second);
}
/*
void Object2D::SpriteSet(const char* name){
    try{
        sprite=SpritesTotal.at(name);
    }catch(const std::out_of_range&){
        sprite=SpritesTotal.begin()->second;
    }
}*/