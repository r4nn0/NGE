#include "Input.h"
namespace Input{
    double mouse_x=0;
    double mouse_y=0;
    bool button[GLFW_MOUSE_BUTTON_LAST]={ 0 };
    bool button_pressed[GLFW_MOUSE_BUTTON_LAST]={ 0 };
    bool button_released[GLFW_MOUSE_BUTTON_LAST]={ 0 };

    void mousePosCallback(GLFWwindow* window, double _x, double _y){
        mouse_x=_x;
        mouse_y=_y;
    }
    void mouseButtonCallback(GLFWwindow* window, int _button, int action, int mods){
        if(_button<0)
            return;
        if(action != GLFW_RELEASE && button[_button]==false){
            button_pressed[_button]=true;
            button_released[_button]=false;
        }
        if(action == GLFW_RELEASE && button[_button]==true){
            button_pressed[_button]=false;
            button_released[_button]=true;
        }
        button[_button] = action!=GLFW_RELEASE;

    }


    bool mouse_check_button(int _button){
        bool x = button[_button];
        if(!x){button[_button]=false;}
        return x;
    }
    bool mouse_check_button_pressed(int _button){
        bool x = button_pressed[_button];
        button_pressed[_button]=false;
        return x;
    }
    bool mouse_check_button_released(int _button){
        bool x = button_released[_button];
        button_released[_button]=false;
        return x;
    }
    /* Keyboard Input */
    bool key_pressed[GLFW_KEY_LAST]={0};
    bool key_held[GLFW_KEY_LAST]={0};
    bool key_released[GLFW_KEY_LAST]={0};

    void keyboardCallback(GLFWwindow* window,int key, int scancode, int action, int mods){
        if (key<0)
            return;
        if(action == GLFW_PRESS && key_held[key]==false){
            key_pressed[key]=true;
            key_released[key]=false;
        }
        if(action == GLFW_RELEASE && key_held[key]==true){
            key_pressed[key]=false;
            key_released[key]=true;
        }
        key_held[key] = action!=GLFW_RELEASE;
    }
    bool keyboard_check_pressed(int _vk){
        bool x=key_pressed[_vk];
        key_pressed[_vk]=false;
        return x;
    }
    bool keyboard_check(int _vk){
        bool x=key_held[_vk];
        if(!x){key_held[_vk]=false;}
        return x;
    }
    bool keyboard_check_released(int _vk){
        bool x=key_released[_vk];
        key_released[_vk]=false;
        return x;
    }
}
