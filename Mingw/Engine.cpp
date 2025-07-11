#include "Engine.h"
glm::mat4 Engine::projMat=glm::mat4(0.f);
glm::mat4 Engine::viewMat=glm::mat4(0.f);
glm::mat4 Engine::orthoMat=glm::mat4(0.f);
glm::mat4 Engine::viewMat2D=glm::mat4(0.f);
float Engine::znear2D=-128;
float Engine::zfar2D=127;
Camera3D Engine::camera3d;
bool Engine::isCursorHidden = false;
Engine::Engine() : view_xview(0), view_yview(0),
                   background_color(100,100,100){}
/**
 * @brief Initialize the engine by creating a rendering window
 * 
 * @param window_title Window name
 * @param _window_width Window Width
 * @param _window_height Window Height
 * @return true Window Created successfully
 * @return false returns 0 and displays an error
 */

bool Engine::init(const char* window_title, int _window_width, int _window_height){
    if(!glfwInit()){
        std::cout << "Error Initializing GLFW" << std::endl;
        return false;
    }
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 5);
    //glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    //window_width = _window_width;
    //window_height= _window_height;
    window=glfwCreateWindow(_window_width, _window_height, window_title, NULL, NULL);
    
    if(window==NULL){
        std::cout << "Error Creating Window" << std::endl;
        return false;
    }
    
    glfwMakeContextCurrent(window);
    if(glewInit()!=GLEW_OK){
        std::cout << "Error Initializing GLEW" << std::endl;
        return false;
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
    int xPos=(mode->width - _window_width)/2;
    int yPos=(mode->height - _window_height)/2;
    glfwSetWindowPos(window, xPos, yPos);
    

    glViewport(0, 0, _window_width, _window_height);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    /*
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW); // Or GL_CW depending on your model's winding order
    */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_WRITEMASK);

    
    projMat=glm::perspective(45.0f, (float)_window_width/_window_height,0.1f,1000.0f);
    orthoMat=glm::ortho(0.f, (float)_window_width, (float)_window_height, 0.f, -znear2D, -(zfar2D+1));
    
    return true;
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
    glViewport(0, 0, width, height);
    Engine::setProjMatrix(glm::perspective(45.0f,(float)width/height,0.1f,1000.0f));
    Engine::setOrthoMatrix(glm::ortho(0.f, (float)width, (float)height, 0.f, -Engine::znear2D, -(Engine::zfar2D+1)));
}
void windowFocusCallback(GLFWwindow* window, int focus){
    
    if(focus){
        if(Engine::isCursorHidden)
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
    
    viewMat = camera3d.getMatrix();
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
const float* Engine::getViewMatrix(){
    return (const float*)glm::value_ptr(viewMat);
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
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return glm::vec2(width, height);
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
/**
 * @brief Compiles a shader depending on the type (vertex or fragment)
 * 
 * @param type type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
 * @param source path to shader
 * @return unsigned int id of the shader
 */
unsigned int Engine::CompileShader(unsigned int type, const char* source){
    unsigned int id = glCreateShader(type);

    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS,&result);
    if(!result){
        int length;
        glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length);
        //char *errMessage=(char*)malloc(length*sizeof(char));
        char errMessage[length];
        glGetShaderInfoLog(id, length,&length, errMessage);
        std::cout << "Couldn't Load " << (type==GL_VERTEX_SHADER?"Vertex":"Fragment")<<"Shader"<< std::endl;
        std::cout << errMessage << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}
/**
 * @brief Creates a full shader from text (Used after LoadShaderFromFile)
 * 
 * @param vertexShader vertex shader as text
 * @param fragmentShader fragment shader as text
 * @return unsigned int returns the id of the shader program to use
 */
unsigned int Engine::CreateShader(const char*vertexShader, const char*fragmentShader){

    unsigned int program=glCreateProgram();

    unsigned int vs=CompileShader(GL_VERTEX_SHADER,vertexShader);
    unsigned int fs=CompileShader(GL_FRAGMENT_SHADER,fragmentShader);
    glAttachShader(program,vs);
    glAttachShader(program,fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}
/**
 * @brief Loads a shader from file and saves it as text
 * 
 * @param ShaderPath Path to shader
 * @return std::string shader as string
 */
std::string Engine::LoadShaderFromFile(const char* ShaderPath){
    FILE* file=fopen(ShaderPath,"rt");
    fseek(file,0,SEEK_END);
    unsigned long length=ftell(file);
    char* data=new char[length+1];
    memset(data,0,length+1);
    fseek(file,0,SEEK_SET);
    fread(data,1,length,file);
    fclose(file);
    std::string src(data);
    delete [] data;
    return src;
}
