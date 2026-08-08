#include "Renderer.h"
Renderer::Renderer() : toFlush(0){
    unsigned shader3D = CreateShader(LoadShaderFromFile("./Graphics/Shaders/shader3D.vs").c_str(),
                                     LoadShaderFromFile("./Graphics/Shaders/shader3D.fs").c_str());
    unsigned shader2D = CreateShader(LoadShaderFromFile("./Graphics/Shaders/shader.vs").c_str(),
                                     LoadShaderFromFile("./Graphics/Shaders/shader.fs").c_str());
    unsigned fontShader = CreateShader(LoadShaderFromFile("./Graphics/Shaders/fontShader.vs").c_str(),
                                     LoadShaderFromFile("./Graphics/Shaders/fontShader.fs").c_str());
    unsigned primShader = CreateShader(LoadShaderFromFile("./Graphics/Shaders/primShader.vs").c_str(),
                                     LoadShaderFromFile("./Graphics/Shaders/primShader.fs").c_str());

    renderer3D.LoadShader(shader3D);
    renderer2D.LoadShader(shader2D);
    textRenderer.LoadShader(fontShader);
    primRenderer.LoadShader(primShader);
}
void Renderer::ReloadShaders(uint8_t shaders){
    unsigned shader; 
    if(shaders & 1){
        shader = CreateShader(LoadShaderFromFile("./Graphics/Shaders/shader.vs").c_str(),
                              LoadShaderFromFile("./Graphics/Shaders/shader.fs").c_str());
        renderer2D.LoadShader(shader);
    }
    if(shaders & 2){
        shader = CreateShader(LoadShaderFromFile("./Graphics/Shaders/fontShader.vs").c_str(),
                              LoadShaderFromFile("./Graphics/Shaders/fontShader.fs").c_str());
        textRenderer.LoadShader(shader);
    }
    if(shaders & 4){
        shader = CreateShader(LoadShaderFromFile("./Graphics/Shaders/primShader.vs").c_str(),
                              LoadShaderFromFile("./Graphics/Shaders/primShader.fs").c_str());
        primRenderer.LoadShader(shader);
    }
    if(shaders & 8){
        shader = CreateShader(LoadShaderFromFile("./Graphics/Shaders/shader3D.vs").c_str(),
                              LoadShaderFromFile("./Graphics/Shaders/shader3D.fs").c_str());
        renderer3D.LoadShader(shader);
    }
}
Renderer::~Renderer(){

}

void Renderer::Flush(){

    if(toFlush & 1)
        renderer2D.flush();
    if(toFlush & 2)
        textRenderer.flush();
    if(toFlush & 4)
        primRenderer.flush();
    if(toFlush & 8)
        renderer3D.flush();
}