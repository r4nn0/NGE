#ifndef GAME_ENGINE
#define GAME_ENGINE

/*--------------------------------------
#define MB_LEFT GLFW_MOUSE_BUTTON_1
#define MB_RIGHT GLFW_MOUSE_BUTTON_2
#define MB_MIDDLE GLFW_MOUSE_BUTTON_3
--------------------------------------*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Input.h"
#include "ngetype.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#pragma once
class Engine{
public:
    static int SCREEN_WIDTH;
    static int SCREEN_HEIGHT;
    bool init(const char* window_title, int _view_xport, int _view_yport);

    void StepEvent();
    void BeginDraw();
    void EndDraw();
    static int view_xport, view_yport, view_width, view_height, view_xview, view_yview;
    static glm::vec3 background_color;
    GLFWwindow* get_window();
    static unsigned int CreateShader(const char*, const char*);
    std::string LoadShaderFromFile(const char*);
    const float* getOthroMatrix();
    
    

private:
    static GLFWwindow* window;
    static GLFWwindow* window2;
    static unsigned int CompileShader(unsigned int type, const char*source);
    float right, left, bottom, top, near, far;
    glm::mat4 ortho_mat;

};




#endif
