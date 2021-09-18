#include "Engine.h"
#include "Graphics/Sprite.h"
#include "Graphics/stb_image.h"
#include "gmath.h"
#include "ngetype.h"
#include "Graphics/Renderer2D.h"
#include "FTGL/ftgl.h"
#include "Graphics/TexturePage.h"


int main (){
    glm::vec3 BACKGROUND_COLOR(100.0/255,100.0/255,100.0/255);
    Engine *gameEngine = new Engine;

    gameEngine->init("NGE", 640,480);
    FTGLBitmapFont font("C:/Windows/Fonts/arial.ttf");

	Renderer2D renderer;

    Sprite spr("./Assets/Sprites/sonic.png", glm::vec3(100, 64, 0));
    Sprite spr2("./Assets/Sprites/sprWall.png", glm::vec3(64, 64, 0));



    std::string vshader=gameEngine->LoadShaderFromFile("./Graphics/Shaders/shader.vs"),
                fshader=gameEngine->LoadShaderFromFile("./Graphics/Shaders/shader.fs");
    unsigned int shader=gameEngine->CreateShader(vshader.c_str(),fshader.c_str());


    double prevTime = glfwGetTime();
    unsigned short FPS = 0;
    std::string fpsString = "0";
    TexturePage tp;
    tp.ImageResizeCanvas(90, 50, 4);
    tp.ImageAdd(&spr);
    tp.ImageAdd(&spr2);
    tp.Bind(0);
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
        glUniform1i(glGetUniformLocation(shader, "texture"), 0);

        renderer.renderBegin();

        for (int i = 0;i < 1000;i++) {
            renderer.addSprite(&spr);
            renderer.addSprite(&spr2);
        }
        renderer.renderEnd();
        renderer.Render();


        glUseProgram(0);

        gameEngine->EndDraw();
        renderer.dcpf = 0;
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
