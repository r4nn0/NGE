#include "Engine.h"
#include "Graphics/Sprite.h"
#include "Graphics/stb_image.h"
#include "gmath.h"
#include "ngetype.h"
#include "Graphics/Renderer2D.h"
#include "FTGL/ftgl.h"
#include "Graphics/TexturePage.h"

int main (){
    glm::vec3 BACKGROUND_COLOR(100.0/255.0);
    Engine gameEngine;

    gameEngine.init("NGE", 1024,768);
    //If the app crashes try using a font that is located in the same directory as the app
    FTGLBitmapFont font("C:/Windows/Fonts/arial.ttf");
	Renderer2D renderer;

    //TexturePage tp;
    //After Creating the tp you need to resize it so you can load sprites to it
    //ex: tp.ImageResizeCanvas(100/*width*/, 100/*height*/,4/*number of channels*/ );
    //Now you need to create a sprite and pass it to the tp
    //ex: Sprite spr("C:/Path/To/Sprite.png", glm::vec3(0,0,0)/*Position: where it should be rendered*/);
    //tp.ImageAdd(&spr);
    //Don't forget to bind the texture in the tp (tp.Bind(0/*Texture Slot, Range from 0 to 32*/);)
    //If you haven't bound any other texture you don't need to worry about the last step
    //Scroll down to pass the sprite to the renderer
    
    std::string vshader=gameEngine.LoadShaderFromFile("./Graphics/Shaders/shader.vs"),
                fshader=gameEngine.LoadShaderFromFile("./Graphics/Shaders/shader.fs");
    unsigned int shader=gameEngine.CreateShader(vshader.c_str(),fshader.c_str());


    double prevTime = glfwGetTime();
    unsigned short FPS = 0;
    std::string fpsString = "0";
    
    while(!glfwWindowShouldClose(gameEngine.get_window())){
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
        gameEngine.BeginDraw();
        
        font.FaceSize(16);
        font.Render(fpsString.c_str(),-1,FTPoint(0,600-font.FaceSize(),0));
        
        glUseProgram(shader);
        glUniformMatrix4fv(glGetUniformLocation(shader, "proj_matrix"),1,GL_FALSE,gameEngine.getOthroMatrix());
        glUniform1i(glGetUniformLocation(shader, "texture"), 0);

        renderer.renderBegin();
        //renderer.addSprite(&spr);
        //renderer.addSprite(&spr);
        //Here you can render sprites depending on MAX_SPRITE_COUNT macro in Renderer2D.h
        //changing the number won't affect the performance but it won't render anything
        //if you render more sprites than MAX_SPRITE_COUNT and it might crash
        renderer.renderEnd();
        renderer.Render();


        glUseProgram(0);

        gameEngine.EndDraw();
        renderer.dcpf = 0;
    }
    
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
