#include "Engine.h"
glm::mat4 Engine::ortho_mat=glm::mat4(0.f);
Engine::Engine() : window_width(800), window_height(600),
                   view_xport(0), view_yport(0),
                   view_xview(0), view_yview(0),
                   view_wport(0), view_hport(0),
                   view_width(0),view_height(0),
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
    window_width = _window_width;
    window_height= _window_height;
    window=glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
    
    if(window==NULL){
        std::cout << "Error Creating Window" << std::endl;
        return false;
    }
    view_width=window_width;
    view_height=window_height;
    
    view_wport = window_width;
    view_hport = window_height;
    
    glfwMakeContextCurrent(window);
    if(glewInit()!=GLEW_OK){
        std::cout << "Error Initializing GLEW" << std::endl;
        return false;
    }
    // Toggle VSYNC
    glfwSwapInterval(1);
	
    glfwSetKeyCallback(window,keyboardCallback);
    glfwSetCursorPosCallback(window,mousePosCallback);
    glfwSetMouseButtonCallback(window,mouseButtonCallback);
	glfwSetJoystickCallback(joystickCallback);
    const GLFWvidmode* mode= glfwGetVideoMode(glfwGetPrimaryMonitor());
    int xPos=(mode->width - window_width)/2;
    int yPos=(mode->height - window_height)/2;
    glfwSetWindowPos(window, xPos, yPos);
    

    /// CAMERA
    glViewport(view_xport, view_yport, view_wport, view_hport);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,0,0,0,-10,10);
    glDepthRange(-64,63);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    left=view_xview;
    right=view_width+view_xview;
    bottom=view_height+view_yview;
    top=view_yview;
    near=-10;
    far=10;
    
    return true;
}
/**
 * @brief Updates the view and values for the callbacks functions to work properly
 * 
 */
void Engine::StepEvent(){
    glfwPollEvents();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom, top, near, far);
    ortho_mat=glm::ortho(left,right,bottom,top,near,far);
    glMatrixMode(GL_MODELVIEW);
}
/**
 * @brief Initializes rendering by clearing the rendering screen
 * 
 */
void Engine::BeginDraw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(100/255.0,100/255.0,100/255.0,1);
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
const float* Engine::getOthroMatrix(){
    return (const float*)glm::value_ptr(ortho_mat);
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
    return glm::vec2(window_width, window_height);
}
/**
 * @brief Get current view width
 * 
 * @return int view_width
 */
int Engine::getViewWidth(){
    return view_width;
}
/**
 * @brief Get current view height
 * 
 * @return int view_height
 */
int Engine::getViewHeight(){
    return view_height;
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