#ifndef OBJECT2D_HEADER
#define OBJECT2D_HEADER
#include "Graphics/Renderer2D.h"

class Object2D{
public:
    Object2D(std::string);
    Object2D();
    ~Object2D();
    void Update();
    void Render();
    void SpriteSet(std::string);
    glm::vec2 position;
    glm::vec2 scale;
    glm::vec4 color;
    float frame_index, anim_speed;
    std::string sprite_name;
    CollisionBox bbox;
protected:
    Sprite* sprite;
};

#endif