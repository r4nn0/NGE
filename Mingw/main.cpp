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


int main (){
    glm::vec3 BACKGROUND_COLOR(25.f/255.f);
    Engine gameEngine;
    
    gameEngine.init("NGE", 1600,900);
    LoadSpritesToMemroy();
    //LoadModelsToMemory();

    //Skybox skybox;
	Renderer2D renderer;
    //Renderer3D renderer3D;
    
    
    TextRenderer tr;
    TestPlayer obj2D("sonic_roll");
    //Entity obj("test");
    
    std::wstring fpsString = L"0";
    /*float actdir= -60;
    float actpitch=60;
    float hsp=-10, vsp=-10, zsp=-1;
    float sense = 0.12f;*/
    float hsp=0, vsp=0;
    const float desiredFPS = 200;
    
    gameEngine.setBackgroundColor(BACKGROUND_COLOR);
    
    float fpsReal=0;
    auto prevTime=std::chrono::steady_clock::now();
    
    while(!glfwWindowShouldClose(gameEngine.get_window())){
        float LX = gamepad_axis_value(0, GLFW_GAMEPAD_AXIS_LEFT_X) > 0.5 ? 1 : gamepad_axis_value(0, GLFW_GAMEPAD_AXIS_LEFT_X) <-0.5 ? -1 : 0;
        float LY = gamepad_axis_value(0, GLFW_GAMEPAD_AXIS_LEFT_Y) > 0.5 ? 1 : gamepad_axis_value(0, GLFW_GAMEPAD_AXIS_LEFT_Y) <-0.5 ? -1 : 0;

        float RX = gamepad_axis_value(0, GLFW_GAMEPAD_AXIS_RIGHT_X) > 0.5 ? 1 : gamepad_axis_value(0, GLFW_GAMEPAD_AXIS_RIGHT_X) <-0.5 ? -1 : 0;
        float RY = gamepad_axis_value(0, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) > 0.5 ? 1 : gamepad_axis_value(0, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) <-0.5 ? -1 : 0;
        //std::cout << "LX: " << LX << "\nLY: " << LY << "\nRX: " << RX << "\nRY: " << RY << std::endl;
        auto currTime=std::chrono::steady_clock::now();
        std::chrono::duration<float> dt = currTime-prevTime;
        prevTime=currTime;
        fpsReal = 1.f / dt.count();
        static float deltaTime = 1;
        if(fpsReal>0.0f)
            deltaTime = desiredFPS/fpsReal;
        // Render fps text every half a second
        static double updatestrFPS = 0;
        updatestrFPS++;
        if(updatestrFPS>=(fpsReal/5.0)){
            fpsString = std::to_wstring((int)gmath::round(fpsReal));
            updatestrFPS=0;
        }
        //std::cout << "FPS: " <<fpsReal <<  std::endl;
        if (keyboard_check_pressed(GLFW_KEY_ESCAPE))
            break;
        /*
        if(keyboard_check_pressed('L'))
            gameEngine.ToggleCursorVisibility();
        glfwSetCursorPos(gameEngine.get_window(),gameEngine.getViewWidth()/2, gameEngine.getViewHeight()/2);
        
        actdir+=(mouse_x-gameEngine.getViewWidth()/2) * sense + (RX);
        if(actdir>360) actdir -= 360;
        if(actdir<0) actdir += 360;
        float dir = glm::radians(actdir);
        actpitch+=(mouse_y-gameEngine.getViewHeight()/2) * sense + (RY);
        if (actpitch > 60.0f) actpitch = 60.0f;
        if (actpitch < -60.0f) actpitch = -60.0f;
        float pitch = glm::radians(actpitch);

        gameEngine.camera3d.setPosition(glm::vec3(hsp,zsp,vsp));
        gameEngine.camera3d.setRotation(glm::vec3(dir , pitch, glm::radians(180.f)));
        gameEngine.camera3d.setOrbit(glm::vec3(hsp,zsp,vsp));
        

        float moveSpeed = 0.03 * deltaTime;
        float turnSpeed = 1.2;
        if(keyboard_check(GLFW_KEY_LEFT_SHIFT) || keyboard_check(GLFW_KEY_RIGHT_SHIFT))
            moveSpeed=0.1 * deltaTime;
        int moving = keyboard_check('W') - keyboard_check('S');
        
        
        hsp -= (((keyboard_check('D') - keyboard_check('A')) * glm::cos(dir)
             +(keyboard_check('W') - keyboard_check('S')) * glm::sin(dir)) + (LX * glm::cos(dir) - LY*glm::sin(dir))) * moveSpeed;
        vsp += (((keyboard_check('D') - keyboard_check('A')) * glm::sin(dir)
             -(keyboard_check('W') - keyboard_check('S')) * glm::cos(dir))  + (LX * glm::sin(dir) + LY*glm::cos(dir)))* moveSpeed;
        zsp +=(keyboard_check('E') - keyboard_check('Q')) * moveSpeed;
        
        truckRot += glm::radians((keyboard_check('Z') - keyboard_check('X'))*0.1f);
        */
        
        
        gameEngine.StepEvent();
        char hinput = (keyboard_check('D') - keyboard_check('A')) | (char)(LX);
        char vinput = (keyboard_check('S') - keyboard_check('W')) | (char)(LY);
        hsp = hinput * 3 * deltaTime;
        vsp = vinput * 3 * deltaTime;
        obj2D.Update();
        obj2D.position += glm::vec3(hsp, vsp, 0.0f);
        obj2D.scale = glm::vec2(3);
        obj2D.anim_speed = 0.15;
        gameEngine.BeginDraw();
        //skybox.Render();
        //obj.Render();
        /*NOTE: You can only render after Engin::BeginDraw call and before Engine::EndDraw call*/
        
        
        obj2D.Render();
        
        tr.setFontColor(glm::vec4(1.0,1.0,1.0,0.5));
        tr.setFontSize(2);
        tr.setValign(Align::CENTER);
        tr.setHalign(Align::MIDDLE);
        tr.renderText(ar_fix(L"مرحبا هذه تجربة"), 800, 450, 0);

        
        
        renderer.Render();
        tr.flush();
        gameEngine.EndDraw();
        
    }
    
    glfwTerminate();
    return 0;
}
