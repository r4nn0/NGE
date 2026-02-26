#ifndef SKYBOX_RENDERER_H
#define SKYBOX_RENDERER_H
#include "../Sprite.h"

class Skybox{
public:
    Skybox();
    void Render();
private:
    unsigned m_VAO, m_VBO, m_Shader, m_TextureID;
    std::vector<float> skyboxVertices;
};

#endif