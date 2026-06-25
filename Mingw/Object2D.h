#ifndef OBJECT2D_HEADER
#define OBJECT2D_HEADER
#include <Graphics/Sprite.h>
class Object2D{
public:
    Object2D(std::string="");
    ~Object2D();
    virtual void Update();
    void Render();
    void SpriteSet(std::string);
    glm::vec3 position;
    glm::vec2 origin;
    glm::vec2 scale;
    float rotation;
    glm::vec4 color;
    float frame_index, anim_speed;
    int textureSlot;
    std::vector<std::vector<glm::vec2>> UVs;
    std::vector<std::vector<bool>> collisionMask;
    inline glm::vec2& getSpriteSize(){return sprite_size[frame_index];}
protected:
    std::vector<glm::vec2> sprite_size;

};
extern std::vector<Object2D> Objects2DtoRender;
#endif