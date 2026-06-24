#rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
srcFiles 	:= $(wildcard *.cpp)
srcFiles	+= $(wildcard Graphics/*.cpp)
srcFiles	+= $(wildcard Graphics/Renderers/*.cpp)
objFiles	:= $(patsubst %.cpp,%.o,$(srcFiles))
DEPENDS 	:= $(patsubst %.cpp,%.d,$(srcFiles))
Includedirs	:= include .
TARGET		:= 64
Libdirs		:= lib 
Libs		:= tess${TARGET} freetype${TARGET} glew${TARGET} glfw${TARGET} glu32 gdi32 winmm opengl32
LinkLibs	:= -static #use static or dynamic option to link against versions respectivly
Macros		:= GLEW_STATIC
OutFolder	:= bin\x${TARGET}
ObjDBG	 	:= obj-dbg/x${TARGET}
ObjREL	 	:= obj-rel/x${TARGET}
buildFlags	:= 	$(patsubst %, -D%, $(Macros))\
				$(patsubst %, -I%, $(Includedirs))\
				$(patsubst %, -L%, $(Libdirs))
CXX			:= C:/mingw${TARGET}/bin/g++
buildDBG	:= $(CXX) -std=c++20 -Wall -g
buildREL	:= $(CXX) -std=c++20 -Wall -O2 -s
SHELL		:= cmd.exe
relEXE		:= $(OutFolder)/Nge-rel.exe
dbgEXE		:= $(OutFolder)/Nge-dbg.exe
# Debug
$(dbgEXE): $(patsubst %, $(ObjDBG)/%,  $(objFiles))
	$(buildDBG) $(CXXFLAGS) $^ -o $@ $(buildFlags) $(LinkLibs) $(patsubst %, -l%, $(Libs))

# Release
$(relEXE): $(patsubst %, $(ObjREL)/%, $(objFiles))
	$(buildREL) $(CXXFLAGS) $^ -o $@ $(buildFlags) $(LinkLibs) $(patsubst %, -l%, $(Libs))
	
rel: $(relEXE)
dbg: $(dbgEXE)

all: $(relEXE) $(dbgEXE)


run: $(relEXE)
	$^


$(patsubst %, $(ObjREL)/%.o, %): %.cpp
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(buildREL) $(CXXFLAGS) -MMD -MP -c $< -o $@ $(buildFlags)
$(patsubst %, $(ObjDBG)/%.o, %): %.cpp
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(buildDBG) $(CXXFLAGS) -MMD -MP -c $< -o $@ $(buildFlags)

-include $(patsubst %, $(ObjDBG)/%, $(DEPENDS))
-include $(patsubst %, $(ObjREL)/%, $(DEPENDS))

clean:
# powershell -Command "Get-ChildItem -Recurse -Filter *.o | Where-Object { $$_.Name -ne 'headerImpl.o' } | Remove-Item"
	del /S *.o
	del /S *.d
	del $(OutFolder)\*.exe
