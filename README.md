# CppgEngine
Indiemade C++ Engine<br/>
CppgEngine is free open source project built for game developers using C++ <br/>
This Engine is based on OpenGL Libraries, some other libraries included i wrote them my self with the help of compiler's built-in functions to make things easier <br/>
# Supported Operating Systems
<b>[+] Windows</b><br/>
<b>[-] Linux [Soon]</b><br/>
<b>[-] Mac OS [Soon]</b><br/>

# How To Setup
<b>PLEASE DONOT IGNORE THIS TUTORIAL TO NOT FACE ANY TROUBLE WHEN COMPILING</b><br/>
In spite of the project is built with Code::Blocks IDE<br/>
it can be imported to other IDE's but there are some steps<br/>
that should be done to get it fully working.<br/>
make sure to link the following libraries to your compiler<br/>
<ul>
  <li><b>glew32</b> [glew32s for the static library of glew]</li>
  <li><b>freeglut</b> [freeglut_static for the static library of glut]</li>
  <li><b>glfw3dll</b> [glfw3 for the static library of glfw3]</li>
  <li><b>soil</b></li>
  <li><b>cairo</b></li>
  <li><b>gdi32</b></li>
  <li><b>winmm</b></li>
  <li><b>opengl32</b></li>
</ul><br/>
Some Macros should be defined before including any libraries<br/>
they could be defined either before including any library or when compiling<br/>
The -D flag followed by the macro name is an easier way to define it so you don't worry about where to define it<br/>
Macros to be defined are:
<br/>
```
#define GLEW_STATIC
#define FREEGLUT_STATIC
```
<br/>
# How to Use
The engine is supposed to be easy to use and understand but regardless i will try to make a manual for it.<br/>
