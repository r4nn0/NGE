#include "Object2D.h"

std::vector<Object2D> Objects2DtoRender;

Object2D::Object2D(std::string sprName) : position(glm::vec3(0)),
                                          origin(glm::vec2(0)),
                                          scale(glm::vec2(1)),
                                          rotation(0),
                                          color(glm::vec4(1)),
                                          frame_index(0), anim_speed(0),
                                          textureSlot(-1){
    SpriteSet(sprName);
}
/**
 * @brief Destroy the Object2D
 * 
 */
Object2D::~Object2D(){
}
/**
 * @brief Update the object every frame
 * 
 */
void Object2D::Update(){
    frame_index+=anim_speed;
    frame_index = std::fmod(frame_index, UVs.size());
}
/**
 * @brief Add the sprite of the object to the rendering list
 * 
 */
void Object2D::Render(){
    Objects2DtoRender.push_back(*this);
}
/**
 * @brief Set the sprite for the object
 * 
 * @param sprName name of the sprite to use (same name as the files in the Sprites folder without ".ngesprite")
 */
void Object2D::SpriteSet(std::string sprName){
    std::map<std::string, Sprite*>::iterator pos = SpritesTotal.find(sprName);
    
    if(pos==SpritesTotal.end()){
        std::cout << "Sprite not found or not loaded to memory!" << std::endl;
        return;
    }
    Sprite temp = *(pos->second);
    sprite_size = temp.getFramesSize();
    textureSlot = temp.getTextureSlot();
    UVs = temp.getUVs();
    collisionMask = temp.getCollisionMask();
}