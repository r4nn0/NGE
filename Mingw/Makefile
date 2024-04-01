objFiles	:= Engine.o gmath.o Input.o ngetype.o main.o stb_image.o Sprite.o Renderer2D.o TexturePage.o
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

# Release
Nge-rel.exe: $(patsubst %, $(ObjREL)/%, $(objFiles))
	$(buildREL) $(patsubst %, $(ObjREL)/%, $(objFiles)) -o $(OutFolder)/Nge-rel.exe $(buildFlags)
# Engine
obj-rel/Engine.o:
	$(buildREL) -c Engine.cpp -o $(ObjREL)/Engine.o $(buildFlags)
# Gmath
obj-rel/gmath.o: 
	$(buildREL) -c gmath.cpp -o $(ObjREL)/gmath.o $(buildFlags)
# NGE Type
obj-rel/ngetype.o:
	$(buildREL) -c ngetype.cpp -o $(ObjREL)/ngetype.o $(buildFlags)
# Input
obj-rel/Input.o: 
	$(buildREL) -c Input.cpp -o $(ObjREL)/Input.o $(buildFlags)
# Main
obj-rel/main.o: 
	$(buildREL) -c main.cpp -o $(ObjREL)/main.o $(buildFlags)
# Stb Image
obj-rel/stb_image.o:
	$(buildREL) -c Graphics/stb_image.cpp -o $(ObjREL)/stb_image.o $(buildFlags)
# Sprite
obj-rel/Sprite.o: 
	$(buildREL) -c Graphics/Sprite.cpp -o $(ObjREL)/Sprite.o $(buildFlags)
# Renderer2D
obj-rel/Renderer2D.o:
	$(buildREL) -c Graphics/Renderer2D.cpp -o $(ObjREL)/Renderer2D.o $(buildFlags)
# Texture Page
obj-rel/TexturePage.o:
	$(buildREL) -c Graphics/TexturePage.cpp -o $(ObjREL)/TexturePage.o $(buildFlags)

# Debug
Nge-dbg.exe: $(patsubst %, $(ObjDBG)/%, $(objFiles))
	$(buildDBG) $(patsubst %, $(ObjDBG)/%, $(objFiles)) -o $(OutFolder)/Nge-dbg.exe $(buildFlags)
# Engine
obj-dbg/Engine.o:
	$(buildDBG) -c Engine.cpp -o $(ObjDBG)/Engine.o $(buildFlags)
# Gmath
obj-dbg/gmath.o: 
	$(buildDBG) -c gmath.cpp -o $(ObjDBG)/gmath.o $(buildFlags)
# NGE Type
obj-dbg/ngetype.o:
	$(buildDBG) -c ngetype.cpp -o $(ObjDBG)/ngetype.o $(buildFlags)
# Input
obj-dbg/Input.o: 
	$(buildDBG) -c Input.cpp -o $(ObjDBG)/Input.o $(buildFlags)
# Main
obj-dbg/main.o: 
	$(buildDBG) -c main.cpp -o $(ObjDBG)/main.o $(buildFlags)
# Stb Image
obj-dbg/stb_image.o:
	$(buildDBG) -c Graphics/stb_image.cpp -o $(ObjDBG)/stb_image.o $(buildFlags)
# Sprite
obj-dbg/Sprite.o: 
	$(buildDBG) -c Graphics/Sprite.cpp -o $(ObjDBG)/Sprite.o $(buildFlags)
# Renderer2D
obj-dbg/Renderer2D.o:
	$(buildDBG) -c Graphics/Renderer2D.cpp -o $(ObjDBG)/Renderer2D.o $(buildFlags)
# Texture Page
obj-dbg/TexturePage.o:
	$(buildDBG) -c Graphics/TexturePage.cpp -o $(ObjDBG)/TexturePage.o $(buildFlags)

