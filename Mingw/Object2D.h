#ifndef OBJECT2D_HEADER
#define OBJECT2D_HEADER
#include "Graphics/Renderer2D.h"

class Object2D{
public:
    Object2D(std::string="");
    ~Object2D();
    virtual void Update();
    void Render();
    void SpriteSet(std::string);
    glm::vec3 position;
    glm::vec2 scale;
    glm::vec4 color;
    float frame_index, anim_speed;
    const std::string getSpriteName() const {return sprite_name;}
    CollisionBox bbox;
protected:
    std::string sprite_name;
    Sprite* sprite;
};

#endif