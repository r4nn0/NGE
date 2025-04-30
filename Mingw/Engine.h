#ifndef GAME_ENGINE
#define GAME_ENGINE

/*--------------------------------------*/
#define MB_LEFT GLFW_MOUSE_BUTTON_1
#define MB_RIGHT GLFW_MOUSE_BUTTON_2
#define MB_MIDDLE GLFW_MOUSE_BUTTON_3
/*--------------------------------------*/

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Input.h"
#include "gmath.h"
#include "ngetype.h"
#include "Graphics/Camera3D.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#pragma once
class Engine{
public:
    bool init(const char* window_title, int _view_xport, int _view_yport);

    void StepEvent();
    void BeginDraw();
    void EndDraw();
    static Camera3D camera3d;
    GLFWwindow* get_window();
    static unsigned int CreateShader(const char*, const char*);
    static std::string LoadShaderFromFile(const char*);
    static const float* getProjMatrix();
    static const float* getViewMatrix();
    Engine(bool is3D=false);
    void setBackgroundColor(glm::vec3);
    glm::vec2 getWindowSize();
    int getViewWidth();
    int getViewHeight();
    int getViewX();
    int getViewY();
private:
    int window_width, window_height,
    view_xport, view_yport,
    view_xview, view_yview,
    view_wport, view_hport,
    view_width, view_height;
    
    bool is3D;
    
    GLFWwindow* window;
    static unsigned int CompileShader(unsigned int type, const char*source);
    float right, left, bottom, top, near, far;
    static glm::mat4 projMat;
    static glm::mat4 viewMat;
    glm::vec3 background_color;
};




#endif
