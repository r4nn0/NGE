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
                   view_xview(0), view_yview(0),
                   background_color(100,100,100),
                   projMat(glm::mat4(0.f)),orthoMat(glm::mat4(0.f)),
                   viewMat2D(glm::mat4(0.f)), window_size(glm::vec2(_window_width, _window_height)),
                   window(nullptr), znear2D(-128), zfar2D(127), isCursorHidden(false){

    if(!glfwInit()){
        std::cout << "Error Initializing GLFW" << std::endl;
        return;
    }
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 5);
    //glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    //window_width = _window_width;
    //window_height= _window_height;
    window=glfwCreateWindow(_window_width, _window_height, window_title, NULL, NULL);
    
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
	glfwSetJoystickCallback(joystickCallback);
    const GLFWvidmode* mode= glfwGetVideoMode(glfwGetPrimaryMonitor());
    std::cout << mode->refreshRate << "hz" << std::endl;
    int xPos=(mode->width - _window_width)/2;
    int yPos=(mode->height - _window_height)/2;
    glfwSetWindowPos(window, xPos, yPos);
    

    glViewport(0, 0, _window_width, _window_height);
    
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

    
    projMat=glm::perspective(45.0f, (float)_window_width/_window_height,0.1f,100000.0f);
    orthoMat=glm::ortho(0.f, (float)_window_width, (float)_window_height, 0.f, -znear2D, -(zfar2D+1));
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
    Engine::getInstance().setProjMatrix(glm::perspective(45.0f,(float)width/height,0.1f,100000.0f));
    Engine::getInstance().setOrthoMatrix(glm::ortho(0.f, (float)width, (float)height, 0.f, -Engine::getInstance().znear2D, -(Engine::getInstance().zfar2D+1)));
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
    viewMat2D = glm::translate(glm::mat4(1.0f), glm::vec3(-view_xview, view_yview, 0.0f));
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
 * @brief Get current view matrix
 * 
 * @return const float* point to the matrix
 */
const float* Engine::getProjMatrix(){
    return (const float*)glm::value_ptr(projMat);
}
void Engine::setProjMatrix(glm::mat4 mat){
    projMat = mat;
}
const float* Engine::getOrthoMatrix(){
    return (const float*)glm::value_ptr(orthoMat);
}
void Engine::setOrthoMatrix(glm::mat4 mat){
    orthoMat = mat;
}
const float* Engine::getViewMatrix2D(){
    return (const float*)glm::value_ptr(viewMat2D);
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
/**
 * @brief Get current view width
 * 
 * @return int view_width
 */
int Engine::getViewWidth(){
    return glm::ceil(glm::abs(2.0f/orthoMat[0][0]));
}
/**
 * @brief Get current view height
 * 
 * @return int view_height
 */
int Engine::getViewHeight(){
    return glm::ceil(glm::abs(2.0f/orthoMat[1][1]));
}
/**
 * @brief Get current view X coordinate 
 * 
 * @return int view x poisition
 */
int Engine::getViewX(){
    return view_xview;
}
/**
 * @brief Get current view Y coordinate 
 * 
 * @return int view y poisition
 */
int Engine::getViewY(){
    return view_yview;
}
