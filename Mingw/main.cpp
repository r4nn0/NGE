#include "Engine.h"
#include "Graphics/Sprite.h"
#include "Graphics/stb_image.h"
#include "gmath.h"
#include "ngetype.h"
#include "Graphics/Renderer2D.h"
#include "FTGL/ftgl.h"


int main (){
    glm::vec3 BACKGROUND_COLOR(100.0/255,100.0/255,100.0/255);
    Engine *gameEngine = new Engine;

    gameEngine->init("NGE", 640,480);
    FTGLBitmapFont font("C:/Windows/Fonts/arial.ttf");


	Renderer2D renderer;




    std::string vshader=gameEngine->LoadShaderFromFile("./Graphics/Shaders/shader.vs"),
                fshader=gameEngine->LoadShaderFromFile("./Graphics/Shaders/shader.fs");
    unsigned int shader=gameEngine->CreateShader(vshader.c_str(),fshader.c_str());


    double prevTime = glfwGetTime();
    unsigned short FPS = 0;
    std::string fpsString = "0";

    while(!glfwWindowShouldClose(gameEngine->get_window())){
        if (keyboard_check_pressed(GLFW_KEY_ESCAPE))
            break;
        double currTime = glfwGetTime();
        FPS++;
        if ( currTime - prevTime >= 1.0 ){
            fpsString=std::to_string(FPS);
            FPS=0;
            prevTime=currTime;
        }

        gameEngine->background_color=BACKGROUND_COLOR;
        gameEngine->StepEvent();

        gameEngine->BeginDraw();


        font.FaceSize(16);
        font.Render(fpsString.c_str(),-1,FTPoint(0,600-font.FaceSize(),0));

        glUseProgram(shader);

        glUniformMatrix4fv(glGetUniformLocation(shader, "proj_matrix"),1,GL_FALSE,gameEngine->getOthroMatrix());
        glUniform1i(glGetUniformLocation(shader,"tex"),0);

        renderer.renderBegin();
        //renderer.addSprite(&Sprite);
        renderer.renderEnd();
        renderer.Render();

        glUseProgram(0);

        gameEngine->EndDraw();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
