objFiles	:= Engine.o gmath.o Input.o ngetype.o main.o Graphics/stb_image.o Graphics/Sprite.o Graphics/Renderer2D.o Graphics/TexturePage.o
srcFiles	:= $(wildcard *.cpp)
srcFiles	+= $(wildcard Graphics/*.cpp)
Includedirs	:= include
Libdirs		:= lib
Libs		:= ftgl freetype glew32 glfw3 gdi32 winmm opengl32
Macros		:= GLEW_STATIC
OutFolder	:= bin
ObjDBG	 	:= obj-dbg
ObjREL	 	:= obj-rel
buildFlags	:= 	$(patsubst %, -D%, $(Macros))\
				$(patsubst %, -I%, $(Includedirs))\
				$(patsubst %, -L%, $(Libdirs))\
				$(patsubst %, -l%, $(Libs))
buildDBG	:= g++ -std=c++1y -Wall -g
buildREL	:= g++ -std=c++1y -Wall -O2 -s

# Debug
$(OutFolder)/Nge-dbg.exe: $(patsubst %, $(ObjDBG)/%, $(objFiles))
	$(buildDBG) $^ -o $@ $(buildFlags)

# Release
$(OutFolder)/Nge-rel.exe: $(patsubst %, $(ObjREL)/%, $(objFiles))
	$(buildREL) $^ -o $@ $(buildFlags)

all: $(patsubst %, $(ObjREL)/%, $(objFiles)) bin/Nge-rel.exe\
	 $(patsubst %, $(ObjREL)/%, $(objFiles)) bin/Nge-dbg.exe
run: $(OutFolder)/Nge-dbg.exe
	$(OutFolder)/Nge-dbg.exe
# Engine
$(ObjREL)/Engine.o: Engine.cpp
	$(buildREL) -c $^ -o $@ $(buildFlags)
# Gmath
$(ObjREL)/gmath.o: gmath.cpp
	$(buildREL) -c $^ -o $@ $(buildFlags)
# NGE Type
$(ObjREL)/ngetype.o: ngetype.cpp
	$(buildREL) -c $^ -o $@ $(buildFlags)
# Input
$(ObjREL)/Input.o: Input.cpp
	$(buildREL) -c $^ -o $@ $(buildFlags)
# Main
$(ObjREL)/main.o: main.cpp
	$(buildREL) -c $^ -o $@ $(buildFlags)
# Stb Image
$(ObjREL)/Graphics/stb_image.o: Graphics/stb_image.cpp
	$(buildREL) -c $^ -o $@ $(buildFlags)
# Sprite
$(ObjREL)/Graphics/Sprite.o: Graphics/Sprite.cpp
	$(buildREL) -c $^ -o $@ $(buildFlags)
# Renderer2D
$(ObjREL)/Graphics/Renderer2D.o: Graphics/Renderer2D.cpp
	$(buildREL) -c $^ -o $@ $(buildFlags)
# Texture Page
$(ObjREL)/Graphics/TexturePage.o: Graphics/TexturePage.cpp
	$(buildREL) -c $^ -o $@ $(buildFlags)

# Engine
$(ObjDBG)/Engine.o: Engine.cpp
	$(buildDBG) -c $^ -o $@ $(buildFlags)
# Gmath
$(ObjDBG)/gmath.o: gmath.cpp
	$(buildDBG) -c $^ -o $@ $(buildFlags)
# NGE Type
$(ObjDBG)/ngetype.o: ngetype.cpp
	$(buildDBG) -c $^ -o $@ $(buildFlags)
# Input
$(ObjDBG)/Input.o: Input.cpp
	$(buildDBG) -c $^ -o $@ $(buildFlags)
# Main
$(ObjDBG)/main.o: main.cpp
	$(buildDBG) -c $^ -o $@ $(buildFlags)
# Stb Image
$(ObjDBG)/Graphics/stb_image.o: Graphics/stb_image.cpp
	$(buildDBG) -c $^ -o $@ $(buildFlags)
# Sprite
$(ObjDBG)/Graphics/Sprite.o: Graphics/Sprite.cpp
	$(buildDBG) -c $^ -o $@ $(buildFlags)
# Renderer2D
$(ObjDBG)/Graphics/Renderer2D.o: Graphics/Renderer2D.cpp
	$(buildDBG) -c $^ -o $@ $(buildFlags)
# Texture Page
$(ObjDBG)/Graphics/TexturePage.o: Graphics/TexturePage.cpp
	$(buildDBG) -c $^ -o $@ $(buildFlags)

clean:
	del /S *.o
	del $(OutFolder)\*.exe