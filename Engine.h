#ifndef GAME_ENGINE
#define GAME_ENGINE

/*--------------------------------------*/
#define MB_LEFT GLFW_MOUSE_BUTTON_1
#define MB_RIGHT GLFW_MOUSE_BUTTON_2
#define MB_MIDDLE GLFW_MOUSE_BUTTON_3
/*--------------------------------------*/
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include "Assets/Input.h"
#pragma once

class Engine{
public:
    static int SCREEN_WIDTH;
    static int SCREEN_HEIGHT;

    Engine();
    ~Engine();
    bool init(const char* window_title, int _view_xport, int _view_yport);

    void StepEvent();
    void BeginDraw();
    void EndDraw();
    static int view_xport, view_yport, view_width, view_height, view_xview, view_yview;
    GLFWwindow* get_window();
    static unsigned int CreateShader(const char*vertext, const char*fragment);
    std::string LoadShaderFromFile(const std::string&ShaderPath);

private:
    static GLFWwindow* window;
    static unsigned int CompileShader(unsigned int type, const char*source);
};




#endif
