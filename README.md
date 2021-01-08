# NGE (Newbies Game Engine)
Indiemade C++ Engine<br/>
NGE is free open source project built for game developers using C++ <br/>
This Engine is based on OpenGL Libraries, some other libraries included i wrote them my self with the help of compiler's built-in functions to make things easier <br/>
# Supported Operating Systems
<b>[+] Windows</b><br/>
<b>[\~] Linux [Soon]</b><br/>
<b>[\~] Mac OS [Soon]</b><br/>

# How To Setup
### PLEASE DO NOT IGNORE THIS TUTORIAL TO NOT FACE ANY TROUBLE WHEN COMPILING
#### Note: you can use the builddbg.bat (for a console version)
#### and buildrel.bat (for a release version)
#### files to build the project but you will have to
#### modify the batch files if you want to compile more files
In spite of the project being built with Code::Blocks IDE<br/>
it can be imported to other IDE's but there are some steps<br/>
that should be done to get it fully working.<br/>
make sure to link the following libraries to your compiler<br/>
<ul>
  <li><b>glew32</b> [glew32s for the static library of glew]</li>
  <li><b>freeglut</b> [freeglut_static for the static library of glut]</li>
  <li><b>glfw3dll</b> [glfw3 for the static library of glfw3]</li>
  <li><b>soil</b></li>
  <li><b>gdi32</b></li>
  <li><b>winmm</b></li>
  <li><b>opengl32</b></li>
</ul><br/>
Some Macros has to be defined when static libraries are linked<br/>
They could be defined either before including any library or at compile time<br/>
Using the -D flag followed by the macro name is an easier way to define it so you don't have to worry about where to define the macros<br/>
Macros to be defined are:<br/>

<h4> &#35define GLEW_STATIC</h4>
<h4> &#35define FREEGLUT_STATIC</h4>

# How To Use
The engine is supposed to be easy to use and understand but regardless i will try to make a manual for it.<br/>
# Features that aren't added yet
Some features are limited in this engine, one of them is the high resolution vector images rendering
its not that important for 2D games but it will be added as soon as possible.
Music and sound effects can't be imported and played yet, but as mentioned they will be added soon or you could add your own libraries manually.

# License
The project is absolutly free to use, any feedback would be greetly appreciated<br/>
support us by subscribing to our youtube channel [GrodRiket Security](https://youtube.com/ITGEEKS)<br/>
you can contact us on [our website](http://grodriket.com/)
