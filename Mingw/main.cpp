#include <algorithm>
#include "FTGL/ftgl.h"
#include "Graphics/Renderer2D.h"
#include <windows.h>
#include "ngestd.h"
#include "TestPlayer.h"
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
    glm::vec3 BACKGROUND_COLOR(100.0/255.0);
    Engine gameEngine;
    
    gameEngine.init("NGE", 1000,500);
    LoadSpritesToMemroy();
    
    
    //If the app crashes try using a font that is located in the same directory as the app
    FTGLPixmapFont font("C:/Windows/Fonts/arial.ttf");
    
	Renderer2D renderer;
    //Sprite spr("sprite.png",glm::vec2(32,32));
    TestPlayer testObject("sonic_walk");
    
    double prevTime = glfwGetTime();
    unsigned short FPS = 0;
    std::string fpsString = "0";
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
        
        gameEngine.setBackgroundColor(BACKGROUND_COLOR);
        gameEngine.StepEvent();
        
        testObject.Update();
        

        gameEngine.BeginDraw();
        
        /*NOTE: You can only render after Engin::BeginDraw call and before Engine::EndDraw call*/
        
        font.FaceSize(16);
        font.Render(fpsString.c_str(),-1,FTPoint(0,gameEngine.getViewHeight()-font.FaceSize(),0));
        font.FaceSize(100);
        font.Render(ar_fix(L"مرحبا").c_str(), -1, FTPoint(gameEngine.getViewWidth()/2-font.FaceSize()/2,gameEngine.getViewHeight()/2-font.FaceSize()/2,0));

        
        testObject.Render();
        
        renderer.Render();
        
        
        gameEngine.EndDraw();
        renderer.dcpf = 0;

        //auto t2 = std::chrono::high_resolution_clock::now();
        //auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        //std::cout << ms_int.count() << std::endl;
    }
    
    glfwTerminate();
    return 0;
}
