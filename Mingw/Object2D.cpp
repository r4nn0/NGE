#include "Object2D.h"
/**
 * @brief Construct a new Object2D
 * 
 * @param sprName the name of the sprite to use for the object
 */
Object2D::Object2D(std::string sprName) : position(glm::vec3(0)),
                                       scale(glm::vec2(1)),
                                       rotation(glm::vec3(0,0,0)),
                                       color(glm::vec4(1)),
                                       frame_index(0), anim_speed(0),
                                       bbox(1), sprite_name(sprName),sprite(nullptr){
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
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), glm::vec3(scale.x, scale.y, 1));
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0),rotation.y, glm::vec3(0,1,0))*
                        glm::rotate(glm::mat4(1.0),rotation.x, glm::vec3(1,0,0))*
                        glm::rotate(glm::mat4(1.0),rotation.z, glm::vec3(0,0,1));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0), position);
    sprite->modelMatrix = translationMatrix*rotationMatrix*scaleMatrix;
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
    sprite = new Sprite();
    
    std::map<std::string, Sprite*>::iterator pos = SpritesTotal.find(sprite_name);
    
    if(pos==SpritesTotal.end()){
        std::cout << "Sprite not found or not loaded to memory!" << std::endl;
        return;
    }
    
    *sprite = *(pos->second);
}