#include <algorithm>
#include "FTGL/ftgl.h"
#include "Graphics/Renderer2D.h"
#include "Graphics/Renderer3D.h"
#include <glm/gtx/string_cast.hpp>
#include <windows.h>
#include "ngestd.h"
#include "TestPlayer.h"
/*
#include <vulkan/vulkan.h>
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
    glm::vec3 BACKGROUND_COLOR(100.0/255.0);
    Engine gameEngine;
    
    gameEngine.init("NGE", 1000,500);
    LoadSpritesToMemroy();
    //MainTextureAtlas.ImageResizeCanvas(512,512);
    //If the app crashes try using a font that is located in the same directory as the app
    FTGLPixmapFont font("C:/Windows/Fonts/arial.ttf");
    
	Renderer2D renderer;
    Renderer3D renderer3D;
    //Sprite spr(glm::vec2(32,32),glm::vec2(512,512));
    TestPlayer testObject("sonic_run");
    Object3D obj("cube.glb");
    Object3D test;
    obj.name="obj";
    test.name="test";
    double prevTime = glfwGetTime();
    unsigned short FPS = 0;
    std::string fpsString = "0";
    float actdir=-45;
    float actpitch=-60;
    float hsp=-10, vsp=-10;
    float sense = 0.1f;
    //spr.setUV();
    test.samplePlane2D();

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
        //SpritesToRender.push_back(spr);
        int _vp[4];
        glGetIntegerv(GL_VIEWPORT, _vp);
        glfwSetCursorPos(gameEngine.get_window(),_vp[2]/2, _vp[3]/2);
        
        actdir-=(mouse_x-_vp[2]/2) * sense;
        if(actdir>360) actdir = 0;
        if(actdir<0) actdir = 360;
        float dir = glm::radians(actdir);

        actpitch-=(mouse_y-_vp[3]/2) * sense;
        if (actpitch > 60.0f) actpitch = 60.0f;
        if (actpitch < -60.0f) actpitch = -60.0f;
        float pitch = glm::radians(actpitch);

        if(keyboard_check_pressed('L'))
            gameEngine.ToggleCursorVisibility();
        
        float moveSpeed = 0.2;
        if(keyboard_check(GLFW_KEY_LEFT_SHIFT) || keyboard_check(GLFW_KEY_RIGHT_SHIFT))
            moveSpeed=0.5;
        hsp += ((keyboard_check('D') - keyboard_check('A')) * glm::sin(dir)
             +(keyboard_check('W') - keyboard_check('S')) * glm::cos(dir)) * moveSpeed;
        vsp -= ((keyboard_check('D') - keyboard_check('A')) * glm::cos(dir)
             -(keyboard_check('W') - keyboard_check('S')) * glm::sin(dir)) * moveSpeed;
        /*
        glm::vec3 newTarget;
        newTarget.x = cos(glm::radians(dir))*cos(glm::radians(pitch));
        newTarget.y = sin(glm::radians(pitch));
        newTarget.z = sin(glm::radians(dir))*cos(glm::radians(pitch));

        gameEngine.camera3d.setTarget(glm::normalize(newTarget));
        glm::vec3 right = glm::cross(gameEngine.camera3d.getTarget(), gameEngine.camera3d.getUp());
        gameEngine.camera3d.setUp(glm::normalize(glm::cross(right, gameEngine.camera3d.getTarget())));*/
        gameEngine.camera3d.setPosition(glm::vec3(hsp , -5.f, vsp));
        gameEngine.camera3d.setTarget(glm::vec3(glm::cos(dir)+hsp, -glm::sin(pitch)*5.f-5,glm::sin(dir) + vsp));
        gameEngine.camera3d.setUp(glm::vec3(0,-1,0));
        gameEngine.setBackgroundColor(BACKGROUND_COLOR);
        gameEngine.StepEvent();

        //if(keyboard_check(GLFW_KEY_W)) pitch+=3;
        //else if(keyboard_check(GLFW_KEY_S)) pitch-=3;
        //float angle = glfwGetTime();
        //glm::vec4 test = glm::vec4(20,32,10,1) * glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //std::cout << test.x << std::endl;
        //test.rotation = glm::vec3(glm::radians(-90.f), 0,0);
        //test.scale=glm::vec3(10,1,10);
        obj.rotation = glm::vec3(glm::radians(-90.f), 0, 0);
        obj.position = 0.2f*glm::vec3(testObject.position.x, testObject.position.y, pitch);
        //test.position = 0.2f*glm::vec3(testObject.position.x, testObject.position.y, pitch);
        //test.position = 0.1f*glm::vec3(0);
        //obj.setModelMatrix(glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
        //obj.setModelMatrix(glm::rotate(obj.getModelMatrix(), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        //obj.setModelMatrix(glm::rotate(obj.getModelMatrix(), angle, glm::vec3(0.0f, 1.0f, 0.0f)));
        //obj.setModelMatrix(glm::translate(glm::mat4(1.0), glm::vec3(1000, 0, 0)));
        //obj.setModelMatrix(glm::rotate(obj.getModelMatrix(), angle, glm::vec3(0.0f, 1.0f, 0.0f)));
        //vertex.model= glm::rotate(vertex.model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        testObject.Update();
        gameEngine.BeginDraw();
        
        /*NOTE: You can only render after Engin::BeginDraw call and before Engine::EndDraw call*/
        // ngestd::DrawRectangle(testObject.bbox.left, testObject.bbox.top, testObject.bbox.right, testObject.bbox.bottom, true);
        font.FaceSize(16);
        font.Render(fpsString.c_str(),-1,FTPoint(0,gameEngine.getViewHeight()-font.FaceSize(),0));
        //font.FaceSize(100);
        //font.Render(ar_fix(L"مرحبا").c_str(), -1, FTPoint(gameEngine.getViewWidth()/2-font.FaceSize()/2,gameEngine.getViewHeight()/2-font.FaceSize()/2,0));

        //testObject.Render();
        //renderer.Render();
        
        
        ObjectsToRender.push_back(obj);
        ObjectsToRender.push_back(test);
        //ObjectsToRender.push_back(obj);
        renderer3D.Render();
        
        gameEngine.EndDraw();
        renderer3D.dcpf = 0;

        //auto t2 = std::chrono::high_resolution_clock::now();
        //auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        //std::cout << ms_int.count() << std::endl;
    }
    
    glfwTerminate();
    return 0;
}
