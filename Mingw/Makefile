srcFiles	:= $(wildcard *.cpp)
srcFiles	+= $(wildcard Graphics/*.cpp)
objFiles	:= $(srcFiles:.cpp=.o)
Includedirs	:= "include" #"C:\VulkanSDK\1.4.313.2\Include"
Libdirs		:= "lib" #"C:\VulkanSDK\1.4.313.2\Lib"
Libs		:= ftgl64 freetype64 glew64 glfw364 glu32 gdi32 winmm opengl32 #
LinkLibs	:= -static #use static or dynamic option to link against versions respectivly
Macros		:= GLEW_STATIC
OutFolder	:= bin
ObjDBG	 	:= obj-dbg
ObjREL	 	:= obj-rel
buildFlags	:= 	$(patsubst %, -D%, $(Macros))\
				$(patsubst %, -I%, $(Includedirs))\
				$(patsubst %, -L%, $(Libdirs))
CXX			:= C:/mingw64/bin/g++
buildDBG	:= $(CXX) -std=c++1y -Wall -g
buildREL	:= $(CXX) -std=c++1y -Wall -O2 -s
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
