srcFiles	:= $(wildcard *.cpp)
srcFiles	+= $(wildcard Graphics/*.cpp)
objFiles	:= $(srcFiles:.cpp=.o)
Includedirs	:= include
Libdirs		:= lib
Libs		:= ftgl freetype glew32 glfw3 glu32 OpenAL32 gdi32 winmm opengl32
LinkLibs	:= -static #dynamic to use dynamic versions of libraries
Macros		:= GLEW_STATIC AL_LIBTYPE_STATIC
OutFolder	:= bin
ObjDBG	 	:= obj-dbg
ObjREL	 	:= obj-rel
buildFlags	:= 	$(patsubst %, -D%, $(Macros))\
				$(patsubst %, -I%, $(Includedirs))\
				$(patsubst %, -L%, $(Libdirs))
buildDBG	:= g++ -std=c++1y -Wall -g
buildREL	:= g++ -std=c++1y -Wall -O2 -s
SHELL		:= cmd.exe

# Debug
$(OutFolder)/Nge-dbg.exe: $(patsubst %, $(ObjDBG)/%, $(objFiles))
	$(buildDBG) $^ -o $@ $(buildFlags) $(LinkLibs) $(patsubst %, -l%, $(Libs))

# Release
$(OutFolder)/Nge-rel.exe: $(patsubst %, $(ObjREL)/%, $(objFiles))
	$(buildREL) $^ -o $@ $(buildFlags) $(LinkLibs) $(patsubst %, -l%, $(Libs))
rel: $(OutFolder)/Nge-rel.exe
dbg: $(OutFolder)/Nge-dbg.exe

all: $(OutFolder)/Nge-rel.exe $(OutFolder)/Nge-dbg.exe


run: $(OutFolder)/Nge-dbg.exe
	$(OutFolder)/Nge-dbg.exe



$(patsubst %, $(ObjREL)/%.o, %): %.cpp 
	$(buildREL) -c $< -o $@ $(buildFlags)
$(patsubst %, $(ObjDBG)/%.o, %): %.cpp 
	$(buildDBG) -c $< -o $@ $(buildFlags)

clean:
	del /S *.o
	del $(OutFolder)\*.exe
