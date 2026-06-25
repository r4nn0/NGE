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
#include "Graphics/Camera3D.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#pragma once
void windowSizeCallback(GLFWwindow*, int, int);
void windowFocusCallback(GLFWwindow*, int);
class Engine{
public:
    static Engine& getInstance(const char* = "NGE", int=800, int=600);
    Engine(const char*, int, int);
    void StepEvent();
    void BeginDraw();
    void EndDraw();
    void ToggleCursorVisibility();
    float znear2D, zfar2D;
    Camera3D camera3d;
    bool isCursorHidden;
    GLFWwindow* get_window();
    
    const float* getProjMatrix();
    void setProjMatrix(glm::mat4);
    const float* getOrthoMatrix();
    void setOrthoMatrix(glm::mat4);
    const float* getViewMatrix2D();
    void setBackgroundColor(glm::vec3);
    glm::vec2 getWindowSize();
    int getViewWidth();
    int getViewHeight();
    int getViewX();
    int getViewY();
private:
    int view_xview, view_yview;
    glm::vec2 window_size;
    glm::vec3 background_color;

    friend void windowSizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* window;

    glm::mat4 projMat;
    glm::mat4 orthoMat;
    glm::mat4 viewMat2D;

};




#endif
