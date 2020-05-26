#include "Engine.h"
#include "Graphics/Sprite.h"
#include "obj_player.h"
#include "obj_wall.h"
#include "CollisionSystem.h"
#include "Graphics/Drawing.h"
#include "gmath.h"
#include "bitwise.h"
#include "ngetype.h"

int main(){
    ngetype::Color BACKGROUND_COLOR(100,100,100);
    Engine engine;
    engine.init("NGE", 1360,765);
    obj_player testPlayer("Assets/Sprites/sonic.png",1,1,engine.view_xport/2,engine.view_yport/2);
    testPlayer.sprite_xoffset=16;
    testPlayer.sprite_yoffset=16;
    obj_wall testWall("Assets/Sprites/sprWall.png",1,1,engine.view_xport/2-30,engine.view_yport/2+30);
    testWall.sprite_index.xscale=3;
/*
    const char* vshader=engine.LoadShaderFromFile("Graphics/Shaders/shader.vs").c_str();
    const char* fshader=engine.LoadShaderFromFile("Graphics/Shaders/shader.fs").c_str();

    unsigned int shader=engine.CreateShader(vshader,fshader);
*/
    obj_wall cop=CollisionSystem::collision_circle(testPlayer.x,testPlayer.y,16,testWall);
    while(!glfwWindowShouldClose(engine.get_window())){
        if (keyboard_check_pressed(GLFW_KEY_ESCAPE))
            break;
        engine.background_color=BACKGROUND_COLOR;
        engine.StepEvent();
        testPlayer.StepEvent();
        testWall.StepEvent();

        engine.view_xview=testPlayer.x-engine.view_xport/2;
        engine.view_yview=testPlayer.y-engine.view_yport/2;

        engine.BeginDraw();

//        glUseProgram(shader);
        testPlayer.DrawEvent();
//        glUseProgram(0);
        testWall.DrawEvent();
        engine.EndDraw();
    }
//    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
