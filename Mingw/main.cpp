#include <algorithm>
#include "Graphics/Renderers/Renderer2D.h"
#include "Graphics/Renderers/Renderer3D.h"
#include "Graphics/Renderers/TextRenderer.h"
#include "Graphics/Renderers/SkyboxRenderer.h"
#include "Graphics/Renderers/PrimitiveRenderer.h"
#include "Graphics/Renderer.h"
#include <glm/gtx/string_cast.hpp>
#include <windows.h>
#include "ngestd.h"
#include "TestPlayer.h"
#include "Entity.h"
#include <locale>
#include <codecvt>
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
    glm::vec3 BACKGROUND_COLOR(0.25,0.25,0.25);
    Engine& gameEngine = Engine::getInstance("NGE");
    gameEngine.setBackgroundColor(BACKGROUND_COLOR);


    LoadSpritesToMemroy();
    LoadModelsToMemory();

    Renderer renderer;
    
    
    
    Object2D obj("sonic_run");
    Object2D wall("wall");
    Entity map("Woman");
    //Entity test("test");
    std::wstring fpsString = L"0";
    
    float actdir= -60;
    float actpitch=60;
    float sense = 0.12f;
    float hsp = 0, vsp=0, zsp=-2.5;
    const float desiredFPS = 200;
    
    Camera& cam = gameEngine.camera;
    
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
        if(updatestrFPS>=fpsReal){
            fpsString = std::to_wstring((int)gmath::round(fpsReal));
            updatestrFPS=0;
        }
        //std::cout << "FPS: " <<fpsReal <<  std::endl;
        if (keyboard_check_pressed(GLFW_KEY_ESCAPE))
            break;
        
        
        if(keyboard_check_pressed('L'))
            gameEngine.ToggleCursorVisibility();
        glfwSetCursorPos(gameEngine.get_window(),cam.view_size.x/2, cam.view_size.y/2);
        //cam.orbitDistance = 10;
        actdir+=(mouse_x-cam.view_size.x/2) * sense + (RX);
        if(actdir>360) actdir -= 360;
        if(actdir<0) actdir += 360;
        float dir = glm::radians(actdir);
        actpitch+=(mouse_y-cam.view_size.y/2) * sense + (RY);
        if (actpitch > 60.0f) actpitch = 60.0f;
        if (actpitch < -60.0f) actpitch = -60.0f;
        float pitch = glm::radians(actpitch);

        
        

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
        
        cam.position3D = glm::vec3(hsp,zsp,vsp);
        cam.rotation3D = glm::vec3(dir , pitch, glm::radians(180.f));
        cam.orbit = glm::vec3(hsp, zsp, vsp);
        //gameEngine.camera3d.setPosition(glm::vec3(hsp,zsp,vsp));
        //gameEngine.camera3d.setRotation(glm::vec3());
        //gameEngine.camera3d.setOrbit(glm::vec3(hsp,zsp,vsp));
        //truckRot += glm::radians((keyboard_check('Z') - keyboard_check('X'))*0.1f);
        //cam.ZoomAt(glm::vec2(scrollY*0.003), glm::vec2(mouse_x, mouse_y));
        //cam.zoom2D = glm::clamp(cam.zoom2D, glm::vec2(1.0f), glm::vec2(100.f));
        
        gameEngine.StepEvent();
        
        char hinput = (keyboard_check('D') - keyboard_check('A')) | (char)(LX) | (keyboard_check(GLFW_KEY_RIGHT) - keyboard_check(GLFW_KEY_LEFT)) | (char)(RX);
        char vinput = (keyboard_check('S') - keyboard_check('W')) | (char)(LY) | (keyboard_check(GLFW_KEY_DOWN) - keyboard_check(GLFW_KEY_UP)) | (char)(RY);
        /*hsp = hinput * 3 * deltaTime;
        vsp = vinput * 3 * deltaTime;
        float zsp = (keyboard_check('Z') - keyboard_check('X')) * 3 * deltaTime;
        */

        obj.position += glm::vec3(hinput, vinput,0.0) * 3.0f * deltaTime;
        obj.scale = glm::vec2(2.5);
        obj.anim_speed = 0.03 * deltaTime;
        obj.Update();
        gameEngine.BeginDraw();
        
        if(keyboard_check('R')) renderer.ReloadShaders(15);
        /*NOTE: You can only render after Engin::BeginDraw call and before Engine::EndDraw call*/
        
        //std::wstring pos = to_wstring(glm::to_string(cam.GetForward()));
        Text text(fpsString, glm::vec3(32,32,0));
        text.col = glm::vec4(0.5,0.0,0.5,1.0);
        
        renderer.Render(text);
        renderer.Render(obj);
        //renderer.Render(wall);
        map.rotation.x = glm::radians(180.0f);
        //map.rotation.y += glm::radians(hinput*3.0f);
        map.UpdateAnimation(1.0);
        //map.scale = glm::vec3(1,-1,1);
        renderer.Render(map);
        
        renderer.Flush();
        
        gameEngine.EndDraw();
        
    }
    
    glfwTerminate();
    return 0;
}
