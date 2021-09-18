#include "Engine.h"
GLFWwindow* Engine::window = NULL;
GLFWwindow* Engine::window2 = NULL;
int Engine::SCREEN_WIDTH=800;
int Engine::SCREEN_HEIGHT=600;
int Engine::view_xport=0;
int Engine::view_yport=0;
int Engine::view_width=SCREEN_WIDTH;
int Engine::view_height=SCREEN_HEIGHT;
int Engine::view_xview=0;
int Engine::view_yview=0;
glm::vec3 Engine::background_color(100,100,100);

/// Initializes Rendering Engine
bool Engine::init(const char* window_title, int _view_xport, int _view_yport){


    if(!glfwInit()){
        std::cout << "Error Initializing GLFW" << std::endl;
        return false;
    }


    window=glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, window_title, NULL, NULL);
    
    
    if(window==NULL){
        std::cout << "Error Creating Window" << std::endl;
        return false;
    }
    


    if (_view_xport>=SCREEN_WIDTH)
        view_xport=_view_xport;
    else
        view_xport=SCREEN_WIDTH;
    if (_view_yport>=SCREEN_HEIGHT)
        view_yport=_view_yport;
    else
        view_yport=SCREEN_HEIGHT;
    glfwMakeContextCurrent(window);
    if(glewInit()!=GLEW_OK){
        std::cout << "Error Initializing GLEW" << std::endl;
        return false;
    }
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glfwSwapInterval(0);
    glfwSetKeyCallback(window,keyboardCallback);
    glfwSetCursorPosCallback(window,mousePosCallback);
    glfwSetMouseButtonCallback(window,mouseButtonCallback);

    const GLFWvidmode* mode= glfwGetVideoMode(glfwGetPrimaryMonitor());
    int xPos=(mode->width - SCREEN_WIDTH)/2;
    int yPos=(mode->height - SCREEN_HEIGHT)/2;
    glfwSetWindowPos(window, xPos, yPos);
    glViewport(0,SCREEN_HEIGHT-view_yport,view_xport,view_yport);

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


unsigned int Engine::CompileShader(unsigned int type, const char* source){
    unsigned int id = glCreateShader(type);

    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS,&result);
    if(!result){
        int length;
        glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length);
        char *errMessage=(char*)malloc(length*sizeof(char));
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
