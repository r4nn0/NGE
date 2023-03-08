srcFiles	:= "Engine.cpp" "gmath.cpp" "Input.cpp" "ngetype.cpp" "main.cpp" "Graphics/stb_image.cpp" "Graphics/Sprite.cpp" "Graphics/Renderer2D.cpp" "Graphics/TexturePage.cpp"
Includedirs	:= "include"
Libdirs		:= "lib"
Libs		:= "ftgl" "freetype" "GL" "GLEW" "glfw"
Macros		:= ""
OutFolder	:= "bin"
CmdDbg		:= g++ -Wall -g $(srcFiles) -o $(OutFolder)/NGE-dbg.bin\
				$(patsubst %, -D%, $(Macros))\
				$(patsubst %, -I%, $(Includedirs))\
				$(patsubst %, -L%, $(Libdirs))\
				$(patsubst %, -l%, $(Libs))
CmdRel		:= g++ -Wall -O2 -s $(srcFiles) -o $(OutFolder)/NGE-rel.bin\
				$(patsubst %, -D%, $(Macros))\
				$(patsubst %, -I%, $(Includedirs))\
				$(patsubst %, -L%, $(Libdirs))\
				$(patsubst %, -l%, $(Libs))

builddbg:
	$(CmdDbg)
buildrel:
	$(CmdRel)
all-rel:
	$(CmdRel) & $(OutFolder)/NGE-rel.bin
all-dbg:
	$(CmdDbg) & $(OutFolder)/NGE-dbg.bin
run-rel:
	$(OutFolder)/NGE-rel.bin
run-dbg:
	$(OutFolder)/NGE-dbg.bin