#include "Engine.h"
GLFWwindow* Engine::window = NULL;
int Engine::SCREEN_WIDTH=800;
int Engine::SCREEN_HEIGHT=600;
int Engine::view_xport=0;
int Engine::view_yport=0;
int Engine::view_width=SCREEN_WIDTH;
int Engine::view_height=SCREEN_HEIGHT;
int Engine::view_xview=0;
int Engine::view_yview=0;
Engine::Engine(){

}
Engine::~Engine(){

}
bool Engine::init(const char* window_title, int _view_xport, int _view_yport){
    int argc = 1;
    char *argv[1] = {(char*)""};
    glutInit(&argc, argv);
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
    glewInit();
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window,Input::keyboardCallback);
    glfwSetCursorPosCallback(window,Input::mousePosCallback);
    glfwSetMouseButtonCallback(window,Input::mouseButtonCallback);

    const GLFWvidmode* mode= glfwGetVideoMode(glfwGetPrimaryMonitor());
    int xPos=(mode->width - SCREEN_WIDTH)/2;
    int yPos=(mode->height - SCREEN_HEIGHT)/2;
    glfwSetWindowPos(window, xPos, yPos);
    glViewport(0,SCREEN_HEIGHT-view_yport,view_xport,view_yport);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /// CAMERA MOVE
    glOrtho(0,0,0,0,-10,10);
    glDepthRange(-10,10);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void Engine::StepEvent(){
    glfwPollEvents();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(view_xview,view_width+view_xview,view_height+view_yview,view_yview,-10,10);
    glMatrixMode(GL_MODELVIEW);
}
void Engine::BeginDraw(){
    glClearColor(.75,.75,.75,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
void Engine::EndDraw(){
    glfwSwapBuffers(window);
}
GLFWwindow* Engine::get_window(){
    return window;
}
unsigned int Engine::CompileShader(unsigned int type, const char*source){
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
std::string Engine::LoadShaderFromFile(const std::string&ShaderPath){
    std::ifstream stream(ShaderPath);
    std::string line;
    std::stringstream ss;
    while(getline(stream,line)){

        ss<<line<<'\n';
    }
    return ss.str();
}
