srcFiles	:= "Engine.cpp" "gmath.cpp" "Input.cpp" "ngetype.cpp" "main.cpp" "Graphics/stb_image.cpp" "Graphics/Sprite.cpp" "Graphics/Renderer2D.cpp" "Graphics/TexturePage.cpp"
Includedirs	:= "include"
Libdirs		:= "lib"
Libs		:= "ftgl" "freetype" "glew32" "glfw3" "gdi32" "winmm" "opengl32"
Macros		:= "GLEW_STATIC"
OutFolder	:= "bin"
CmdDbg		:= g++ -Wall -g $(srcFiles) -o $(OutFolder)/NGE-dbg.exe\
				$(patsubst %, -D%, $(Macros))\
				$(patsubst %, -I%, $(Includedirs))\
				$(patsubst %, -L%, $(Libdirs))\
				$(patsubst %, -l%, $(Libs))
CmdRel		:= g++ -Wall -O2 -s $(srcFiles) -o $(OutFolder)/NGE-rel.exe\
				$(patsubst %, -D%, $(Macros))\
				$(patsubst %, -I%, $(Includedirs))\
				$(patsubst %, -L%, $(Libdirs))\
				$(patsubst %, -l%, $(Libs))

builddbg:
	$(CmdDbg)
buildrel:
	$(CmdRel)
all-rel:
	$(CmdRel) & $(OutFolder)\NGE-rel.exe
all-dbg:
	$(CmdDbg) & $(OutFolder)\NGE-dbg.exe
run-rel:
	$(OutFolder)\NGE-rel.exe
run-dbg:
	$(OutFolder)\NGE-dbg.exe