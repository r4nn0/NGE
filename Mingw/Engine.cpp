#include "Engine.h"
Engine& Engine::getInstance(const char* window_title, int window_width, int window_height){
    static Engine instance(window_title, window_width, window_height);
    return instance;
}
/**
 * @brief Initialize the engine by creating a rendering window
 * 
 * @param window_title Window name
 * @param _window_width Window Width
 * @param _window_height Window Height
 * 
 */
Engine::Engine(const char* window_title, int _window_width, int _window_height) :
                   isCursorHidden(false), window_size(glm::vec2(_window_width, _window_height)),
                   background_color(100,100,100), window(nullptr){

    if(!glfwInit()){
        std::cout << "Error Initializing GLFW" << std::endl;
        return;
    }
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 5);
    //glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode= glfwGetVideoMode(monitor);
    std::cout << mode->refreshRate << "hz" << std::endl;
    if(_window_width<=0 || _window_height<=0)
        window_size = glm::vec2(mode->width, mode->height);
    camera.view_size = window_size;
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    window=glfwCreateWindow(window_size.x, window_size.y, window_title, NULL, NULL);
    
    if(window==NULL){
        std::cout << "Error Creating Window" << std::endl;
        return;
    }
    
    glfwMakeContextCurrent(window);
    if(glewInit()!=GLEW_OK){
        std::cout << "Error Initializing GLEW" << std::endl;
        return;
    }
    
    // Toggle VSYNC
    glfwSwapInterval(1);
	
    glfwSetWindowFocusCallback(window, windowFocusCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetKeyCallback(window,keyboardCallback);
    glfwSetCursorPosCallback(window,mousePosCallback);
    glfwSetMouseButtonCallback(window,mouseButtonCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
	glfwSetJoystickCallback(joystickCallback);
    
    
    //int xPos=(mode->width - _window_width)/2;
    //int yPos=(mode->height - _window_height)/2;
    //glfwSetWindowPos(window, 0, 0);
    
    
    
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    /*
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW); // Or GL_CW depending on your model's winding order
    */
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    //glEnable(GL_DEPTH_WRITEMASK);

    glViewport(0, 0, window_size.x, window_size.y);
}



void Engine::ToggleCursorVisibility(){
    if(isCursorHidden == true)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    isCursorHidden = !isCursorHidden;
}
void windowSizeCallback(GLFWwindow* window, int width, int height){
    //view_width = width;
    //view_height = height;
    Engine::getInstance().window_size = glm::vec2(width, height);
    glViewport(0, 0, width, height);
}
void windowFocusCallback(GLFWwindow* window, int focus){
    
    if(focus){
        if(Engine::getInstance().isCursorHidden)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
/**
 * @brief Updates the view and values for the callbacks functions to work properly
 * 
 */
void Engine::StepEvent(){
    glfwPollEvents();
}
/**
 * @brief Initializes rendering by clearing the rendering screen
 * 
 */
void Engine::BeginDraw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(background_color.x, background_color.y, background_color.z, 1.f);
}
/**
 * @brief Swaps rendered frames with current frames (Updates rendered screen)
 * 
 */
void Engine::EndDraw(){
    glfwSwapBuffers(window);
}
/**
 * @brief Get current GLFW window
 * 
 * @return GLFWwindow* a pointer to the GLFW window
 */
GLFWwindow* Engine::get_window(){
    return window;
}

/**
 * @brief Sets background color for the screen
 * 
 * @param bg (glm::vec3) Background color in RGB 
 */
void Engine::setBackgroundColor(glm::vec3 bg){
    background_color=bg;
}
/**
 * @brief Get window size
 * 
 * @return glm::vec2 Size in glm::vec2 (width, height)
 */
glm::vec2 Engine::getWindowSize(){
    return window_size;
}