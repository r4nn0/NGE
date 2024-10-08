#include "Input.h"

double mouse_x=0;
double mouse_y=0;

static bool mouse_held[GLFW_MOUSE_BUTTON_LAST]={ 0 };
static bool mouse_pressed[GLFW_MOUSE_BUTTON_LAST]={ 0 };
static bool mouse_released[GLFW_MOUSE_BUTTON_LAST]={ 0 };

/**
 * @brief Callback function for the mouse position
 * 
 * @param window pointer to the GLFW window
 * @param _x Mouse x position
 * @param _y Mouse y position
 */
void mousePosCallback(GLFWwindow* window, double _x, double _y){
    mouse_x=_x;
    mouse_y=_y;
}
/**
 * @brief Callback function for the mouse buttons\n 
 * Note: Read more about modifier keys on https://www.glfw.org/docs/3.3/group__mods.html
 * @param window pointer to the GLFW window
 * @param _button button index
 * @param action action performed on button
 * @param mods which modifier keys were held down or enabled, 0 if none
 */
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
/**
 * @brief Checks if a mouse button has been pressed
 * 
 * @param _button button value
 * @return true if the button has been pressed
 * @return false if the button hasn't been pressed
 */
bool mouse_check_button_pressed(int _button){
    bool x = mouse_pressed[_button];
    mouse_pressed[_button]=false;
    return x;
}
/**
 * @brief Check if a mouse button is pressed and held
 * 
 * @param _button button value
 * @return true if the button is held
 * @return false if the button is released
 */
bool mouse_check_button(int _button){
    bool x = mouse_held[_button];
    if(!x){mouse_held[_button]=false;}
    return x;
}
/**
 * @brief Check if a mouse button is released
 * 
 * @param _button button value
 * @return true if the button is released
 * @return false if the button is pressed or held
 */
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
/**
 * @brief Set keyboard callback function\n 
 * Note: Read more about modifier keys on https://www.glfw.org/docs/3.3/group__mods.html
 * @param window pointer to the GLFW window
 * @param key key value
 * @param scancode scan code sent by the system
 * @param action action performed on key
 * @param mods which modifier keys were held down or enabled, 0 if none
 */
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
/**
 * @brief Check if a key has been pressed
 * 
 * @param _vk key value
 * @return true if the key has been pressed
 * @return false if the key has not been pressed
 */
bool keyboard_check_pressed(int _vk){
    bool x=key_pressed[_vk];
    key_pressed[_vk]=false;
    return x;
}
/**
 * @brief Check if a key has been pressed and held
 * 
 * @param _vk key value
 * @return true if the key has been pressed and held
 * @return false if the key has not been pressed or has been released
 */
bool keyboard_check(int _vk){
    bool x=key_held[_vk];
    if(!x){key_held[_vk]=false;}
    return x;
}
/**
 * @brief Check if a key has been released
 * 
 * @param _vk key value
 * @return true if the key has been released
 * @return false if they key wasn't pressed or has not been released
 */
bool keyboard_check_released(int _vk){
    bool x=key_released[_vk];
    key_released[_vk]=false;
    return x;
}
/**
 * @brief Get the key value of any pressed key
 * 
 * @return int the key code of the pressed key
 */
int keyboard_get_pressed(){
    return key_pressed_code;
}
/* Joystick Input */
static bool connectedGamepads[GLFW_JOYSTICK_LAST+1]={0};
static bool gamepad_pressed[GLFW_JOYSTICK_LAST][GLFW_GAMEPAD_BUTTON_LAST]={{0}};
static bool gamepad_held[GLFW_JOYSTICK_LAST][GLFW_GAMEPAD_BUTTON_LAST]={{0}};
static bool gamepad_released[GLFW_JOYSTICK_LAST][GLFW_GAMEPAD_BUTTON_LAST]={{0}};

/**
 * @brief Callback function for the joystick
 * 
 * @param gamepad_id id of the gamepad
 * @param event Event code, GLFW_CONNECTED if connected, GLFW_DISCONNECTED if gamepad is removed
 */
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
/**
 * @brief Check if gamepad is connected
 * 
 * @param gamepad_id id of the gamepad
 * @return true if the gamepad is plugged in
 * @return false if the gamepad is removed
 */
bool gamepad_is_connected(int gamepad_id){
	return glfwJoystickPresent(gamepad_id);
}
/**
 * @brief Callback function for gamepad buttons
 * 
 * @param gamepad_id gamepad id
 * @param _button button value
 */
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
/**
 * @brief Check if a button is pressed and held on a gamepad
 * 
 * @param gamepad_id id of the gamepad
 * @param _button button value
 * @return true if the button is pressed and held
 * @return false if the button is released
 */
bool gamepad_check(int gamepad_id, int _button){
	gamepadCallback(gamepad_id, _button);
	bool x=gamepad_held[gamepad_id][_button];
    if(!x){gamepad_held[gamepad_id][_button]=false;}
    return x;
}
/**
 * @brief Check if a button is pressed
 * 
 * @param gamepad_id id of the gamepad
 * @param _button button value
 * @return true if the button is pressed
 * @return false if the button has not been pressed
 */
bool gamepad_check_pressed(int gamepad_id, int _button){
	gamepadCallback(gamepad_id, _button);
    bool x=gamepad_pressed[gamepad_id][_button];
    gamepad_pressed[gamepad_id][_button]=false;
    return x;
}
/**
 * @brief Check if a button is pressed
 * 
 * @param gamepad_id id of the gamepad
 * @param _button button value
 * @return true if the button has been released
 * @return false if the button is pressed or held
 */
bool gamepad_check_released(int gamepad_id, int _button){
	gamepadCallback(gamepad_id, _button);
    bool x=gamepad_released[gamepad_id][_button];
    gamepad_released[gamepad_id][_button]=false;
    return x;
}
/**
 * @brief return the value of the gamepad analog sticks or triggers\n 
 * Note: Check the GLFW documentation for the values of the axis -> https://www.glfw.org/docs/3.3/group__gamepad__axes.html
 * @param gamepad_id id of the gamepad
 * @param axis id of the axis
 * @return float value in float
 */
float gamepad_axis_value(int gamepad_id, int axis){
	if(gamepad_is_connected(gamepad_id)){
		int count;
		const float* axes = glfwGetJoystickAxes(gamepad_id, &count);
		return axes[axis];
	}
	return 0;
}