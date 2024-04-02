#include "Engine.h"
Engine::Engine() : window_width(800), window_height(600),
                   view_xport(0), view_yport(0),
                   view_xview(0), view_yview(0),
                   background_color(100,100,100){}
/// Initializes Rendering Engine
bool Engine::init(const char* window_title, int _view_wport, int _view_hport){
    if(!glfwInit()){
        std::cout << "Error Initializing GLFW" << std::endl;
        return false;
    }

    window=glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
    
    if(window==NULL){
        std::cout << "Error Creating Window" << std::endl;
        return false;
    }
    view_width=window_width;
    view_height=window_height;
    
    view_wport = (_view_wport>=window_width) ? window_width : _view_wport;
    view_hport = (_view_hport>=window_height) ? window_height : _view_hport;
    
    glfwMakeContextCurrent(window);
    if(glewInit()!=GLEW_OK){
        std::cout << "Error Initializing GLEW" << std::endl;
        return false;
    }
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    //  VSYNC
    glfwSwapInterval(1);
	
    glfwSetKeyCallback(window,keyboardCallback);
    glfwSetCursorPosCallback(window,mousePosCallback);
    glfwSetMouseButtonCallback(window,mouseButtonCallback);
	glfwSetJoystickCallback(joystickCallback);

    const GLFWvidmode* mode= glfwGetVideoMode(glfwGetPrimaryMonitor());
    int xPos=(mode->width - window_width)/2;
    int yPos=(mode->height - window_height)/2;
    glfwSetWindowPos(window, xPos, yPos);
    glViewport(view_xport,view_yport,view_wport,view_hport);

    /// CAMERA
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
/// Updates Events and Camera every frame
void Engine::StepEvent(){
    glfwPollEvents();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom, top, near, far);
    ortho_mat=glm::ortho(left,right,bottom,top,near,far);
    glMatrixMode(GL_MODELVIEW);

}
/// Initializes Drawing Frame Every Frame
void Engine::BeginDraw(){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(background_color.r,background_color.g,background_color.b,1);
}
/// Swaps Rendered Frames with current Frames
void Engine::EndDraw(){
    glfwSwapBuffers(window);
}
/// Returns Current Running window
GLFWwindow* Engine::get_window(){
    return window;
}
const float* Engine::getOthroMatrix(){
    return (const float*)glm::value_ptr(ortho_mat);
}
void Engine::setBackgroundColor(glm::vec3 bg){
    background_color=bg;
}
glm::vec2 Engine::getWindowSize(){
    return glm::vec2(window_width, window_height);
}
int Engine::getViewWidth(){
    return view_width;
}
int Engine::getViewHeight(){
    return view_height;
}
int Engine::getViewX(){
    return view_xview;
}
int Engine::getViewY(){
    return view_yview;
}

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
