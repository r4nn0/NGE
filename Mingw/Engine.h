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
void windowSizeCallback(GLFWwindow*, int, int);
void windowFocusCallback(GLFWwindow*, int);
class Engine{
public:
    bool init(const char* window_title, int, int);

    void StepEvent();
    void BeginDraw();
    void EndDraw();
    void ToggleCursorVisibility();
    static float znear2D, zfar2D;
    static Camera3D camera3d;
    static bool isCursorHidden;
    GLFWwindow* get_window();
    static unsigned int CreateShader(const char*, const char*);
    static std::string LoadShaderFromFile(const char*);
    static const float* getProjMatrix();
    static void setProjMatrix(glm::mat4);
    static const float* getViewMatrix();
    static const float* getOrthoMatrix();
    static void setOrthoMatrix(glm::mat4);
    static const float* getViewMatrix2D();
    Engine();
    void setBackgroundColor(glm::vec3);
    glm::vec2 getWindowSize();
    int getViewWidth();
    int getViewHeight();
    int getViewX();
    int getViewY();
private:
    int view_xview, view_yview;
    glm::vec3 background_color;


    GLFWwindow* window;
    static unsigned int CompileShader(unsigned int type, const char*source);
    static glm::mat4 projMat;
    static glm::mat4 viewMat;
    static glm::mat4 orthoMat;
    static glm::mat4 viewMat2D;

};




#endif
