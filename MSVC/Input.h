#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED
#include <GLFW/glfw3.h>

extern void mousePosCallback(GLFWwindow* window, double _x, double _y);
extern void mouseButtonCallback(GLFWwindow* window, int _button, int action, int mods);
extern double mouse_x;
extern double mouse_y;
extern bool mouse_check_button_pressed(int _button);
extern bool mouse_check_button(int _button);
extern bool mouse_check_button_released(int _button);
extern void keyboardCallback(GLFWwindow* window,int key, int scancode, int action, int mods);
extern bool keyboard_check_pressed(int _vk);
extern bool keyboard_check(int _vk);
extern bool keyboard_check_released(int _vk);
extern int keyboard_get_pressed();
	
#endif
