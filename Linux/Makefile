srcFiles	:= $(wildcard *.cpp)
srcFiles	+= $(wildcard Graphics/*.cpp)
objFiles	:= $(srcFiles:.cpp=.o)
Libs		:= ftgl freetype GLEW GL glfw
# LinkLibs	:= -static #dynamic to use dynamic versions of libraries
# Macros		:= GLEW_STATIC AL_LIBTYPE_STATIC
OutFolder	:= bin
ObjDBG	 	:= obj-dbg
ObjREL	 	:= obj-rel
buildDBG	:= g++ -std=c++1y -Wall -g
buildREL	:= g++ -std=c++1y -Wall -O2 -s


# Debug
$(OutFolder)/Nge-dbg: $(patsubst %, $(ObjDBG)/%, $(objFiles))
	$(buildDBG) $^ -o $@ $(LinkLibs) $(patsubst %, -l%, $(Libs))

# Release
$(OutFolder)/Nge-rel: $(patsubst %, $(ObjREL)/%, $(objFiles))
	$(buildREL) $^ -o $@ $(LinkLibs) $(patsubst %, -l%, $(Libs))
rel: $(OutFolder)/Nge-rel
dbg: $(OutFolder)/Nge-dbg

all: $(OutFolder)/Nge-rel $(OutFolder)/Nge-dbg


run: $(OutFolder)/Nge-dbg
	$(OutFolder)/Nge-dbg



$(patsubst %, $(ObjREL)/%.o, %): %.cpp 
	$(buildREL) -c $< -o $@ $(buildFlags)
$(patsubst %, $(ObjDBG)/%.o, %): %.cpp 
	$(buildDBG) -c $< -o $@ $(buildFlags)

clean:
	del /S *.o
	del $(OutFolder)\*.exe
