#ifndef OBJECT2D_HEADER
#define OBJECT2D_HEADER
#include "Graphics/Renderer2D.h"

class Object2D{
public:
    Object2D(const char*);
    Object2D();
    ~Object2D();
    void Update();
    void Render();
    //void Object2D::SpriteSet(const char*);
    glm::vec2 position;
    glm::vec2 scale;
    glm::vec4 color;
    int frame_index, anim_speed;
    std::vector<std::reference_wrapper<Sprite>> frames;
protected:
    Sprite sprite;
};

#endif