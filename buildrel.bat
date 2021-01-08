g++.exe -Wall -std=c++1y -m32 -DGLEW_STATIC -DFREEGLUT_STATIC -O2 -std=c++1y -Iinclude -IGraphics/SOIL/src -c "Assets/Input.cpp" -o obj/Release/Assets/Input.o
g++.exe -Wall -std=c++1y -m32 -DGLEW_STATIC -DFREEGLUT_STATIC -O2 -std=c++1y -Iinclude -IGraphics/SOIL/src -c "bitwise.cpp" -o obj/Release/bitwise.o
g++.exe -Wall -std=c++1y -m32 -DGLEW_STATIC -DFREEGLUT_STATIC -O2 -std=c++1y -Iinclude -IGraphics/SOIL/src -c "Engine.cpp" -o obj/Release/Engine.o
g++.exe -Wall -std=c++1y -m32 -DGLEW_STATIC -DFREEGLUT_STATIC -O2 -std=c++1y -Iinclude -IGraphics/SOIL/src -c "gmath.cpp" -o obj/Release/gmath.o
g++.exe -Wall -std=c++1y -m32 -DGLEW_STATIC -DFREEGLUT_STATIC -O2 -std=c++1y -Iinclude -IGraphics/SOIL/src -c "Graphics/Sprite.cpp" -o obj/Release/Graphics/Sprite.o
g++.exe -Wall -std=c++1y -m32 -DGLEW_STATIC -DFREEGLUT_STATIC -O2 -std=c++1y -Iinclude -IGraphics/SOIL/src -c "Graphics/Texture.cpp" -o obj/Release/Graphics/Texture.o
g++.exe -Wall -std=c++1y -m32 -DGLEW_STATIC -DFREEGLUT_STATIC -O2 -std=c++1y -Iinclude -IGraphics/SOIL/src -c "main.cpp" -o obj/Release/main.o
g++.exe -Wall -std=c++1y -m32 -DGLEW_STATIC -DFREEGLUT_STATIC -O2 -std=c++1y -Iinclude -IGraphics/SOIL/src -c "ngetype.cpp" -o obj/Release/ngetype.o
g++.exe -Wall -std=c++1y -m32 -DGLEW_STATIC -DFREEGLUT_STATIC -O2 -std=c++1y -Iinclude -IGraphics/SOIL/src -c "obj_player.cpp" -o obj/Release/obj_player.o
g++.exe -Wall -std=c++1y -m32 -DGLEW_STATIC -DFREEGLUT_STATIC -O2 -std=c++1y -Iinclude -IGraphics/SOIL/src -c "obj_wall.cpp" -o obj/Release/obj_wall.o
g++.exe -Wall -std=c++1y -m32 -DGLEW_STATIC -DFREEGLUT_STATIC -O2 -std=c++1y -Iinclude -IGraphics/SOIL/src -c "Objects.cpp" -o obj/Release/Objects.o
g++.exe -Llib-mingw -LGraphics/SOIL/lib -o bin/Release/NGE.exe obj/Release/Assets/Input.o obj/Release/bitwise.o obj/Release/Engine.o obj/Release/gmath.o obj/Release/Graphics/Sprite.o obj/Release/Graphics/Texture.o obj/Release/main.o obj/Release/ngetype.o obj/Release/obj_player.o obj/Release/obj_wall.o obj/Release/Objects.o  -m32 -s  -lftgl_static_D -lfreetype -lfreeglut_static lib-mingw/libglew32s.lib -lglfw3 -lSOIL -lgdi32 -lwinmm -lopengl32 -mwindows
