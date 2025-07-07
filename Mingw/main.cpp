#include <algorithm>
#include "FTGL/ftgl.h"
#include "Graphics/Renderer2D.h"
#include "Graphics/Renderer3D.h"
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

int main(){
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);


    VkInstance instance;
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
createInfo.enabledLayerCount = 0;
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
*/
int main (){
    glm::vec3 BACKGROUND_COLOR(25.f/255.f);
    Engine gameEngine;
    
    gameEngine.init("NGE", 1000,500);
    LoadSpritesToMemroy();
    LoadModelsToMemory();

    //If the app crashes try using a font that is located in the same directory as the app
    FTGLPixmapFont font("C:/Windows/Fonts/arial.ttf");
    
	Renderer2D renderer;
    Renderer3D renderer3D;
    TestPlayer obj2D("sonic_run");
    //testObject.color = glm::vec4(1,0,0,1);

    Entity obj3D("cube");
    Entity floor;
    double prevTime = glfwGetTime();
    unsigned short FPS = 0;
    std::string fpsString = "0";
    float actdir= -60;
    float actpitch=60;
    float hsp=-10, vsp=-10;
    float sense = 0.1f;
    
    
    
    gameEngine.setBackgroundColor(BACKGROUND_COLOR);
    while(!glfwWindowShouldClose(gameEngine.get_window())){
        //auto t1 = std::chrono::high_resolution_clock::now();
        if (keyboard_check_pressed(GLFW_KEY_ESCAPE))
            break;
        double currTime = glfwGetTime();
        FPS++;
        if ( currTime - prevTime >= 1.0 ){
            fpsString=std::to_string(FPS);
            FPS=0;
            prevTime=currTime;
        }
        glfwSetCursorPos(gameEngine.get_window(),gameEngine.getViewWidth()/2, gameEngine.getViewHeight()/2);

        actdir+=(mouse_x-gameEngine.getViewWidth()/2) * sense;
        if(actdir>360) actdir -= 360;
        if(actdir<0) actdir += 360;
        float dir = glm::radians(actdir);

        actpitch+=(mouse_y-gameEngine.getViewHeight()/2) * sense;
        if (actpitch > 60.0f) actpitch = 60.0f;
        if (actpitch < -60.0f) actpitch = -60.0f;
        float pitch = glm::radians(actpitch);

        float moveSpeed = 0.2;
        if(keyboard_check(GLFW_KEY_LEFT_SHIFT) || keyboard_check(GLFW_KEY_RIGHT_SHIFT))
            moveSpeed=0.5;
        hsp -= ((keyboard_check('D') - keyboard_check('A')) * glm::cos(dir)
             +(keyboard_check('W') - keyboard_check('S')) * glm::sin(dir)) * moveSpeed;
        vsp += ((keyboard_check('D') - keyboard_check('A')) * glm::sin(dir)
             -(keyboard_check('W') - keyboard_check('S')) * glm::cos(dir)) * moveSpeed;
        obj3D.position = glm::vec3(hsp, -3, vsp);
        floor.scale = glm::vec3(5,0,5);

        if(keyboard_check_pressed('L'))
            gameEngine.ToggleCursorVisibility();
        
        
        
        gameEngine.camera3d.setPosition(obj3D.position-glm::vec3(0,0,10));
        gameEngine.camera3d.setRotation(glm::vec3(dir , pitch, glm::radians(180.f)));
        gameEngine.camera3d.setOrbit(obj3D.position);
        
        
        
        gameEngine.StepEvent();

        
        obj2D.Update();
        //testObject2.Update();
        
        gameEngine.BeginDraw();
        
        /*NOTE: You can only render after Engin::BeginDraw call and before Engine::EndDraw call*/
        //ngestd::DrawRectangle(glm::vec4(obj2D.bbox.left, obj2D.bbox.top, obj2D.bbox.right, obj2D.bbox.bottom), false);
        font.FaceSize(16);
        font.Render(fpsString.c_str(),-1,FTPoint(0,gameEngine.getViewHeight()-font.FaceSize(),0));

        //font.FaceSize(100);
        //font.Render(ar_fix(L"مرحبا").c_str(), -1, FTPoint(gameEngine.getViewWidth()/2-font.FaceSize()/2,gameEngine.getViewHeight()/2-font.FaceSize()/2,0));

        obj2D.Render();
        
        renderer.Render();
        
        
        obj3D.Render();
        floor.Render();

        renderer3D.Render();
        
        gameEngine.EndDraw();

        //auto t2 = std::chrono::high_resolution_clock::now();
        //auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        //std::cout << ms_int.count() << std::endl;
    }
    
    glfwTerminate();
    return 0;
}
