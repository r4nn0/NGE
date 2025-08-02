srcFiles	:= $(wildcard *.cpp)
srcFiles	+= $(wildcard Graphics/*.cpp)
objFiles	:= $(srcFiles:.cpp=.o)
Includedirs	:= include 
TARGET		:= 32
Libdirs		:= lib 
Libs		:= tess${TARGET} ftgl${TARGET} freetype${TARGET} glew${TARGET} glfw${TARGET} glu32 gdi32 winmm opengl32
LinkLibs	:= -static #use static or dynamic option to link against versions respectivly
Macros		:= GLEW_STATIC
OutFolder	:= bin\x${TARGET}
ObjDBG	 	:= obj-dbg/x${TARGET}
ObjREL	 	:= obj-rel/x${TARGET}
buildFlags	:= 	$(patsubst %, -D%, $(Macros))\
				$(patsubst %, -I%, $(Includedirs))\
				$(patsubst %, -L%, $(Libdirs))
CXX			:= C:/mingw${TARGET}/bin/g++
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
# powershell -Command "Get-ChildItem -Recurse -Filter *.o | Where-Object { $$_.Name -ne 'headerImpl.o' } | Remove-Item"
	del /S *.o
	del $(OutFolder)\*.exe
