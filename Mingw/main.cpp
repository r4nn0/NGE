#include <algorithm>
#include "FTGL/ftgl.h"
#include "Graphics/Renderer2D.h"
#include "Graphics/TexturePage.h"
#include <windows.h>
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
std::wstring ar_alphabet = {
    0xfe8f, 0xfe95, 0xfe99, 0xfe9d,
    0xfea1, 0xfea5, 0xfeb1, 0xfeb5,
    0xfeb9, 0xfebd, 0xfec1, 0xfec5,
    0xfec9, 0xfecd, 0xfed1, 0xfed5,
    0xfed9, 0xfedd, 0xfee1, 0xfee5,
    0xfee9, 0xfef1, 0xfe8d, 0xfea9,
    0xfeab, 0xfead, 0xfeaf, 0xfeed,
    0xfe93, 0xfeef
};
std::wstring  ar_has_mi = {
    1576, 1578, 1579, 1580,
    1581, 1582, 1587, 1588,
    1589, 1590, 1591, 1592,
    1593, 1594, 1601, 1602,
    1603, 1604, 1605, 1606,
    1607, 1610
};
std::wstring  ar_has_no_mi = {
    1575, 1583, 1584,
    1585, 1586, 1608,
    1577, 1609
};

std::wstring  has_mi = {
    0xfe8f, 0xfe95, 0xfe99, 0xfe9d,
    0xfea1, 0xfea5, 0xfeb1, 0xfeb5,
    0xfeb9, 0xfebd, 0xfec1, 0xfec5,
    0xfec9, 0xfecd, 0xfed1, 0xfed5,
    0xfed9, 0xfedd, 0xfee1, 0xfee5,
    0xfee9, 0xfef1
};
std::wstring  has_no_mi = {
    0xfe8d, 0xfea9, 0xfeab,
    0xfead, 0xfeaf, 0xfeed,
    0xfe93, 0xfeef
};

std::wstring ar_fix(std::wstring str){
    for(size_t i = 0; i<str.size();i++){
        
        size_t pos = ar_has_mi.find(str[i]);
        if(pos!=std::wstring::npos) {
            str[i]=has_mi[pos];
            continue;
        }
        pos = ar_has_no_mi.find(str[i]);
        if(pos!=std::wstring::npos) str[i]=has_no_mi[pos];
    }
    std::wstring out = str;
    
    bool cp_has_mi=false;
    for(size_t i = 0; i<str.size();i++){
        wchar_t c = str[i];
        wchar_t cn = str[(i<str.size()) ? i+1:i];
        bool c_has_mi = (has_mi.find(c)!=std::wstring::npos) ? true:false;
        bool cn_is_alphabet = (ar_alphabet.find(cn)!=std::wstring::npos) ? true:false;
        if(c_has_mi){
            if(!cp_has_mi && cn_is_alphabet) c+=2;
            else if(cp_has_mi && cn_is_alphabet) c+=3;
            else if(cp_has_mi) c+=1;
            cp_has_mi=true;
            out[i]=c;
            continue;
        }
        bool c_has_no_mi = (has_no_mi.find(c)!=std::wstring::npos) ? true:false;
        if(cp_has_mi && c_has_no_mi) c++;
        cp_has_mi=false;
        out[i]=c;
    }
    std::wstring temp=out;
    for(size_t j=0;j<out.size();j++){
        out[j]=temp[out.size()-1-j];
    }
    return out;
}
int main (){
    glm::vec3 BACKGROUND_COLOR(100.0/255.0);
    Engine gameEngine;
    
    gameEngine.init("NGE", 1000,500);
    
    //If the app crashes try using a font that is located in the same directory as the app
    FTGLBitmapFont font("C:/Windows/Fonts/arial.ttf");
    
	Renderer2D renderer;
    // Create a texture page 
    TexturePage tp;
    TexturePage tp2(200,200,4,1);

    tp.ImageResizeCanvas(200,200);
    Sprite spr("sprite.png", glm::vec3(32,32,0));
    Sprite wll("wall.png", glm::vec3(200,32,0));
    tp.ImageAdd(&spr);
    tp2.ImageAdd(&wll);
    //After Creating the tp you need to resize it so you can load sprites to it
    //ex: tp.ImageResizeCanvas(100/*width*/, 100/*height*/,4/*number of channels*/ );
    //Now you need to create a sprite and pass it to the tp
    //ex: Sprite spr("C:/Path/To/Sprite.png", glm::vec3(0,0,0)/*Position: where it should be rendered*/);
    //tp.ImageAdd(&spr);
    //Don't forget to bind the texture in the tp (tp.Bind(0/*Texture Slot, Range from 0 to 32*/);)
    //Scroll down to pass the sprite to the renderer
    
    std::string vshader=gameEngine.LoadShaderFromFile("./Graphics/Shaders/shader.vs"),
                fshader=gameEngine.LoadShaderFromFile("./Graphics/Shaders/shader.fs");
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

        
        
        font.FaceSize(16);
        font.Render(fpsString.c_str(),-1,FTPoint(0,gameEngine.getViewHeight()-font.FaceSize(),0));
        font.FaceSize(80);
        //font.Render(L"ﺎﺒﺣﺮﻣ", -1, FTPoint(gameEngine.getViewWidth()/2-font.FaceSize()/2,gameEngine.getViewHeight()/2-font.FaceSize()/2,0));
        //const wchar_t* t= ar_fix(L"ابحرم").c_str();

        //const wchar_t a[6] = {0xfe8e,0xfe92,0xfea3,0xfeae,0xfee3,0};
        const wchar_t* ar = ar_fix(L"مرحبا").c_str();
        
        font.Render(ar, -1, FTPoint(gameEngine.getViewWidth()/2-font.FaceSize()/2,gameEngine.getViewHeight()/2-font.FaceSize()/2,0));
        
        
        
        glUseProgram(shader);
        glUniformMatrix4fv(glGetUniformLocation(shader, "proj_matrix"),1,GL_FALSE,gameEngine.getOthroMatrix());
        /*
         * Make sure to set the uniform "texture" to the texture page slot (use tp.GetTextureSlot() to retrieve the slot number)
         * If you want to use multiple texture pages, make sure to pass each texture page to the shader
         * (See example below)
        */
        glUniform1i(glGetUniformLocation(shader, "texture[0]"), tp.GetTextureSlot());
        glUniform1i(glGetUniformLocation(shader, "texture[1]"), tp2.GetTextureSlot());
        renderer.renderBegin();
        renderer.addSprite(&spr);
        renderer.addSprite(&wll);
        //Here you can render sprites depending on MAX_SPRITE_COUNT macro in Renderer2D.h
        //changing the number won't affect the performance but it won't render anything
        //if you render more sprites than MAX_SPRITE_COUNT and it might crash
        renderer.renderEnd();
        /*Bind the textures page so that the shader can access it*/
        tp.Bind();
        tp2.Bind();
        renderer.Render();
        tp2.Unbind();
        tp.Unbind();
        
        
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
