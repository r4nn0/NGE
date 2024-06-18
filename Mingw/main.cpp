#include <algorithm>
#include "FTGL/ftgl.h"
#include "Graphics/Renderer2D.h"
#include <windows.h>
#include "ngestd.h"
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
    //If the app crashes try using a font that is located in the same directory as the app
    FTGLPixmapFont font("C:/Windows/Fonts/arial.ttf");
    
	Renderer2D renderer;

    Sprite spr("sprite.png", glm::vec3(32,32,0));
    Sprite wll("wall.png", glm::vec3(200,32,0));
    Sprite wll2("wall.png", glm::vec3(200,32,0));
    Sprite wll3("wall.png", glm::vec3(200,32,0));
    
    
    std::string vshader=gameEngine.LoadShaderFromFile("./Graphics/Shaders/shader.vs"),
                fshader=gameEngine.LoadShaderFromFile("./Graphics/Shaders/shader.fs");
                /*
    std::string fntvshader=gameEngine.LoadShaderFromFile("./Graphics/Shaders/fsh.vs"),
                fntfshader=gameEngine.LoadShaderFromFile("./Graphics/Shaders/fsh.fs");
    unsigned int fntshader=gameEngine.CreateShader(fntvshader.c_str(),fntfshader.c_str());
    */
    unsigned int shader=gameEngine.CreateShader(vshader.c_str(),fshader.c_str());
    
    

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
            //spr.setPosition(glm::vec3(gmath::irandom(0,1000-32),gmath::irandom(0,500-32),0));
        }
        
        gameEngine.setBackgroundColor(BACKGROUND_COLOR);
        gameEngine.StepEvent();



        gameEngine.BeginDraw();
        /*NOTE: You can only render after Engin::BeginDraw call and before Engine::EndDraw call*/
        
        font.FaceSize(16);
        font.Render(fpsString.c_str(),-1,FTPoint(0,gameEngine.getViewHeight()-font.FaceSize(),0));
        font.FaceSize(100);
        
        font.Render(ar_fix(L"مرحبا").c_str(), -1, FTPoint(gameEngine.getViewWidth()/2-font.FaceSize()/2,gameEngine.getViewHeight()/2-font.FaceSize()/2,0));
        
        
        SpritesToRender.push_back(spr);
        SpritesToRender.push_back(wll2);
        


        glUseProgram(shader);
        glUniformMatrix4fv(glGetUniformLocation(shader, "proj_matrix"),1,GL_FALSE,gameEngine.getOthroMatrix());
        glUniform1i(glGetUniformLocation(shader, "texture[0]"), MainTextureAtlas.GetTextureSlot());
        renderer.Render();
        glUseProgram(0);
        
        gameEngine.EndDraw();
        renderer.dcpf = 0;

        //auto t2 = std::chrono::high_resolution_clock::now();
        //auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        //std::cout << ms_int.count() << std::endl;
    }
    
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
