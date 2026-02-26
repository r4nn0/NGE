#include <algorithm>
#include "Graphics/Renderers/Renderer2D.h"
#include "Graphics/Renderers/Renderer3D.h"
#include "Graphics/Renderers/TextRenderer.h"
#include "Graphics/Renderers/SkyboxRenderer.h"
#include <glm/gtx/string_cast.hpp>
#include <windows.h>
#include "ngestd.h"
#include "TestPlayer.h"
#include "Entity.h"
//#include <vulkan/vulkan.h>
/*! \mainpage NGE (NewbiesGameEngine)
 *
 * \section intro_sec Introduction
 *
 * This engine is designed for beginners in programming to learn game development in C++
 *
 * \section install_sec Installation
 *
 * \subsection prereq Preperations: 
 * + Make sure you are using Mingw 6.3.0 or later version\n 
 * otherwise you will have to recompile the libraries that are included in the project
 *
 * \subsection comp Compiling:
 * + Open a terminal window in the same folder and run "make" or "make run"\n
 * Note: Check out the makefile to know if you want to compile a debug or a release version.
*/
unsigned skyboxTexture();

int main (){
    
    glm::vec3 BACKGROUND_COLOR(25.f/255.f);
    Engine gameEngine;
    
    gameEngine.init("NGE", 1000,500);
    LoadSpritesToMemroy();
    LoadModelsToMemory();
    
    //If the app crashes try using a font that is located in the same directory as the app
    //FTGLPixmapFont font("C:/Windows/Fonts/arial.ttf");
    Skybox skybox;
	Renderer2D renderer;
    Renderer3D renderer3D;
    TextRenderer tr;
    TestPlayer obj2D("sonic_run");
    //testObject.color = glm::vec4(1,0,0,1);

    Entity obj3D("Duck");

    //Entity sec ("AnimatedMorphSphere");
    //Entity floor;
    double prevTime = glfwGetTime();
    unsigned short FPS = 0;
    std::wstring fpsString = L"0";
    float actdir= -60;
    float actpitch=60;
    float hsp=-10, vsp=-10, zsp=-2;
    float sense = 0.1f;
    float truckRot =0;
    
    gameEngine.setBackgroundColor(BACKGROUND_COLOR);
    
    while(!glfwWindowShouldClose(gameEngine.get_window())){
        //auto t1 = std::chrono::high_resolution_clock::now();
        if (keyboard_check_pressed(GLFW_KEY_ESCAPE))
            break;
        double currTime = glfwGetTime();
        FPS++;
        if ( currTime - prevTime >= 1.0 ){
            fpsString=std::to_wstring(FPS);
            FPS=0;
            prevTime=currTime;
            
        }
        if(keyboard_check_pressed('L'))
            gameEngine.ToggleCursorVisibility();
        glfwSetCursorPos(gameEngine.get_window(),gameEngine.getViewWidth()/2, gameEngine.getViewHeight()/2);
        actdir+=(mouse_x-gameEngine.getViewWidth()/2) * sense;
        if(actdir>360) actdir -= 360;
        if(actdir<0) actdir += 360;
        float dir = glm::radians(actdir);
        actpitch+=(mouse_y-gameEngine.getViewHeight()/2) * sense;
        if (actpitch > 60.0f) actpitch = 60.0f;
        if (actpitch < -60.0f) actpitch = -60.0f;
        float pitch = glm::radians(actpitch);

        gameEngine.camera3d.setPosition(glm::vec3(hsp,zsp,vsp));
        gameEngine.camera3d.setRotation(glm::vec3(dir , pitch, glm::radians(180.f)));
        gameEngine.camera3d.setOrbit(glm::vec3(hsp,zsp,vsp));


        float moveSpeed = 0.05;
        float turnSpeed = 1.2;
        if(keyboard_check(GLFW_KEY_LEFT_SHIFT) || keyboard_check(GLFW_KEY_RIGHT_SHIFT))
            moveSpeed=0.5;
        int moving = keyboard_check('W') - keyboard_check('S');

        hsp -= ((keyboard_check('D') - keyboard_check('A')) * glm::cos(dir)
             +(keyboard_check('W') - keyboard_check('S')) * glm::sin(dir)) * moveSpeed;
        vsp += ((keyboard_check('D') - keyboard_check('A')) * glm::sin(dir)
             -(keyboard_check('W') - keyboard_check('S')) * glm::cos(dir)) * moveSpeed;
        //vsp -= moving *glm::cos(truckRot)* moveSpeed;
        //hsp -= moving *glm::sin(truckRot)* moveSpeed;
        zsp +=(keyboard_check('E') - keyboard_check('Q')) * moveSpeed;
        
        
        truckRot += glm::radians((keyboard_check('D') - keyboard_check('A'))*turnSpeed * moving);
        obj3D.UpdateAnimation(moving*0.005f);
        
        obj3D.rotation=glm::vec3(glm::radians(180.0f), 0,0);
        //obj3D.position=glm::vec3(-15,-15,-15);
        //sec.UpdateAnimation(0.001);
        //sec.position = glm::vec3(50,0,50);*/
        //floor.scale = glm::vec3(5,0,5);

        gameEngine.StepEvent();

        
        obj2D.Update();
        //testObject2.Update();
        
        gameEngine.BeginDraw();
        skybox.Render();
        /*NOTE: You can only render after Engin::BeginDraw call and before Engine::EndDraw call*/
        
        
        obj2D.Render();
        obj3D.Render();
        tr.renderText(fpsString, 5, 15, 0);
        //sec.Render();
        //floor.Render();

        


        
        //tr.renderText(ar_fix(L"مرحبا هذه تجربة"), 100,  100, -1);
        
        renderer3D.Render();
        glClear(GL_DEPTH_BUFFER_BIT);
        renderer.Render();
        tr.flush();
        gameEngine.EndDraw();

        //auto t2 = std::chrono::high_resolution_clock::now();
        //auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        //std::cout << ms_int.count() << std::endl;
    }
    
    glfwTerminate();
    return 0;
}
