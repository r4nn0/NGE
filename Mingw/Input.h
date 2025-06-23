#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED
#include <iostream>
#include <GLFW/glfw3.h>

/// Mouse Functions
extern double mouse_x;
extern double mouse_y;
extern void mousePosCallback(GLFWwindow*, double, double);
extern void mouseButtonCallback(GLFWwindow*, int, int, int);
extern bool mouse_check_button_pressed(int);
extern bool mouse_check_button(int);
extern bool mouse_check_button_released(int);
/// Keyboard Functions
extern void keyboardCallback(GLFWwindow*, int, int, int, int);
extern bool keyboard_check_pressed(short);
extern bool keyboard_check(short);
extern bool keyboard_check_released(short);
extern short keyboard_get_pressed();
/// Joystick Functions
extern void joystickCallback(int, int);
extern bool gamepad_is_connected(int);
extern bool gamepad_check_pressed(int, int);
extern bool gamepad_check(int, int);
extern bool gamepad_check_released(int, int);
extern float gamepad_axis_value(int, int);
#endif
