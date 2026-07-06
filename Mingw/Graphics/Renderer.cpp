#include "Renderer.h"
Renderer::Renderer() : toFlush(0){

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