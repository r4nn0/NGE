#include "Engine.h"
#include "Graphics/Sprite.h"
#include "obj_player.h"
#include "obj_wall.h"
#include "CollisionSystem.h"
#include "Graphics/Drawing.h"
#include "gmath.h"

int main(){
    Engine engine;
    engine.init("NGE", 800,600);
    obj_player testPlayer("Assets/Sprites/sonic.png",1,1,engine.view_xport/2,engine.view_yport/2);
    testPlayer.sprite_xoffset=16;
    testPlayer.sprite_yoffset=16;
    obj_wall testWall("Assets/Sprites/sprWall.png",1,1,engine.view_xport/2-30,engine.view_yport/2+30);
    testWall.sprite_index.xscale=3;

/*
    const char* vshader=engine.LoadShaderFromFile("Graphics/Shaders/shTest.vshader").c_str();
    const char* fshader=engine.LoadShaderFromFile("Graphics/Shaders/shTest.fshader").c_str();

    unsigned int shader=engine.CreateShader(vshader,fshader);
*/
    obj_wall cop=CollisionSystem::collision_circle(testPlayer.x,testPlayer.y,16,testWall);
    while(!glfwWindowShouldClose(engine.get_window())){
        if (keyboard_check_pressed(GLFW_KEY_ESCAPE))
            break;
        engine.StepEvent();
        testPlayer.StepEvent();
        testWall.StepEvent();
        //std::cout << CollisionSystem::collision_line_line(testPlayer.bbox_left,testPlayer.bbox_up,testPlayer.bbox_right,testPlayer.bbox_down,100,100,200,100)<<std::endl;

        //engine.view_xview=testPlayer.x-engine.view_xport/2;
        //engine.view_yview=testPlayer.y-engine.view_yport/2;

        engine.BeginDraw();
        testPlayer.DrawEvent();
        Drawing::draw_circle(testPlayer.x,testPlayer.y,16,true);
        testWall.DrawEvent();


        Drawing::draw_line(testPlayer.bbox_left,testPlayer.bbox_up,testPlayer.bbox_right,testPlayer.bbox_down);
        Drawing::draw_line(100,100,200,100);
        engine.EndDraw();
    }

//  glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
