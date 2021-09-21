#include "Input.h"

double mouse_x=0;
double mouse_y=0;

static bool mouse_held[GLFW_MOUSE_BUTTON_LAST]={ 0 };
static bool mouse_pressed[GLFW_MOUSE_BUTTON_LAST]={ 0 };
static bool mouse_released[GLFW_MOUSE_BUTTON_LAST]={ 0 };

void mousePosCallback(GLFWwindow* window, double _x, double _y){
    mouse_x=_x;
    mouse_y=_y;
}

void mouseButtonCallback(GLFWwindow* window, int _button, int action, int mods){
    if(_button<0)
        return;
    if(action != GLFW_RELEASE && mouse_held[_button]==false){
        mouse_pressed[_button]=true;
        mouse_released[_button]=false;
    }
    if(action == GLFW_RELEASE && mouse_held[_button]==true){
        mouse_pressed[_button]=false;
        mouse_released[_button]=true;
    }
    mouse_held[_button] = action!=GLFW_RELEASE;
}
bool mouse_check_button_pressed(int _button){
    bool x = mouse_pressed[_button];
    mouse_pressed[_button]=false;
    return x;
}
bool mouse_check_button(int _button){
    bool x = mouse_held[_button];
    if(!x){mouse_held[_button]=false;}
    return x;
}
bool mouse_check_button_released(int _button){
    bool x = mouse_released[_button];
    mouse_released[_button]=false;
    return x;
}
/* Keyboard Input */
static int key_pressed_code=0;
static bool key_pressed[GLFW_KEY_LAST]={0};
static bool key_held[GLFW_KEY_LAST]={0};
static bool key_released[GLFW_KEY_LAST]={0};

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
    key_pressed_code=key;
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
int keyboard_get_pressed(){
    return key_pressed_code;
}
/* Joystick Input */
static bool connectedGamepads[GLFW_JOYSTICK_LAST+1]={0};
static bool gamepad_pressed[GLFW_JOYSTICK_LAST][GLFW_GAMEPAD_BUTTON_LAST]={{0}};
static bool gamepad_held[GLFW_JOYSTICK_LAST][GLFW_GAMEPAD_BUTTON_LAST]={{0}};
static bool gamepad_released[GLFW_JOYSTICK_LAST][GLFW_GAMEPAD_BUTTON_LAST]={{0}};

void joystickCallback(int gamepad_id, int event){
	if (event == GLFW_CONNECTED){
		connectedGamepads[gamepad_id] = true;
		std::cout << "Gamepad: " << glfwGetJoystickName(gamepad_id) <<
		" is connected with the id: "<< gamepad_id << " !"
		<< std::endl;
	}
	if (event == GLFW_DISCONNECTED){
		connectedGamepads[gamepad_id] = false;
		std::cout << "Gamepad got disconnected !" << std::endl;
	}
}
bool gamepad_is_connected(int gamepad_id){
	return glfwJoystickPresent(gamepad_id);
}

static void gamepadCallback(int gamepad_id, int _button){
	if(gamepad_is_connected(gamepad_id)){
		int count;
		const unsigned char* buttons = glfwGetJoystickButtons(gamepad_id, &count);
		if(buttons[_button]==GLFW_PRESS && gamepad_held[gamepad_id][_button]==false){
			gamepad_pressed[gamepad_id][_button]=true;
			gamepad_released[gamepad_id][_button]=false;
		}
		else if(buttons[_button]==GLFW_RELEASE && gamepad_held[gamepad_id][_button]==true){
			gamepad_pressed[gamepad_id][_button]=false;
			gamepad_released[gamepad_id][_button]=true;
		}
		gamepad_held[gamepad_id][_button] = buttons[_button]!=GLFW_RELEASE;
	}
}

bool gamepad_check(int gamepad_id, int _button){
	gamepadCallback(gamepad_id, _button);
	bool x=gamepad_held[gamepad_id][_button];
    if(!x){gamepad_held[gamepad_id][_button]=false;}
    return x;
}
bool gamepad_check_pressed(int gamepad_id, int _button){
	gamepadCallback(gamepad_id, _button);
    bool x=gamepad_pressed[gamepad_id][_button];
    gamepad_pressed[gamepad_id][_button]=false;
    return x;
}
bool gamepad_check_released(int gamepad_id, int _button){
	gamepadCallback(gamepad_id, _button);
    bool x=gamepad_released[gamepad_id][_button];
    gamepad_released[gamepad_id][_button]=false;
    return x;
}
float gamepad_axis_value(int gamepad_id, int axis){
	if(gamepad_is_connected(gamepad_id)){
		int count;
		const float* axes = glfwGetJoystickAxes(gamepad_id, &count);
		return axes[axis];
	}
	return 0;
}