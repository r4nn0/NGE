#include "Engine.h"
#include "Graphics/Sprite.h"
#include "obj_player.h"
#include "obj_wall.h"
#include "CollisionSystem.h"
#include "Graphics/Drawing.h"
#include "gmath.h"
#include "bitwise.h"
#include "ngetype.h"

int main (){
    ngetype::Color BACKGROUND_COLOR(100,100,100);
    Engine *gameEngine = new Engine;

    gameEngine->init("NGE", 640,480);


    //Drawing::Font fnt;
    //fnt.init("C:\\WINDOWS\\FONTS\\ARIAL.TTF",71);


//
//    const char* vshader=gameEngine.LoadShaderFromFile("Graphics/Shaders/shader.vs").c_str();
//    const char* fshader=gameEngine.LoadShaderFromFile("Graphics/Shaders/shader.fs").c_str();
//
//    unsigned int shader=gameEngine.CreateShader(vshader,fshader);

    while(!glfwWindowShouldClose(gameEngine->get_window())){

        if (keyboard_check_pressed(GLFW_KEY_ESCAPE))
            break;
        gameEngine->background_color=BACKGROUND_COLOR;
        gameEngine->StepEvent();

        gameEngine->BeginDraw();
        //Render Stuff
        gameEngine->EndDraw();
    }
    //fnt.clean();
    //glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
