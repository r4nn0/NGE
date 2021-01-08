g++.exe -Wall -std=c++1y -m32 -g -DGLEW_STATIC -DFREEGLUT_STATIC -std=c++1y -Iinclude -IGraphics/SOIL/src -c "Assets/Input.cpp" -o obj/Debug/Assets/Input.o
g++.exe -Wall -std=c++1y -m32 -g -DGLEW_STATIC -DFREEGLUT_STATIC -std=c++1y -Iinclude -IGraphics/SOIL/src -c "bitwise.cpp" -o obj/Debug/bitwise.o
g++.exe -Wall -std=c++1y -m32 -g -DGLEW_STATIC -DFREEGLUT_STATIC -std=c++1y -Iinclude -IGraphics/SOIL/src -c "Engine.cpp" -o obj/Debug/Engine.o
g++.exe -Wall -std=c++1y -m32 -g -DGLEW_STATIC -DFREEGLUT_STATIC -std=c++1y -Iinclude -IGraphics/SOIL/src -c "gmath.cpp" -o obj/Debug/gmath.o
g++.exe -Wall -std=c++1y -m32 -g -DGLEW_STATIC -DFREEGLUT_STATIC -std=c++1y -Iinclude -IGraphics/SOIL/src -c "Graphics/Sprite.cpp" -o obj/Debug/Graphics/Sprite.o
g++.exe -Wall -std=c++1y -m32 -g -DGLEW_STATIC -DFREEGLUT_STATIC -std=c++1y -Iinclude -IGraphics/SOIL/src -c "Graphics/Texture.cpp" -o obj/Debug/Graphics/Texture.o
g++.exe -Wall -std=c++1y -m32 -g -DGLEW_STATIC -DFREEGLUT_STATIC -std=c++1y -Iinclude -IGraphics/SOIL/src -c "main.cpp" -o obj/Debug/main.o
g++.exe -Wall -std=c++1y -m32 -g -DGLEW_STATIC -DFREEGLUT_STATIC -std=c++1y -Iinclude -IGraphics/SOIL/src -c "ngetype.cpp" -o obj/Debug/ngetype.o
g++.exe -Wall -std=c++1y -m32 -g -DGLEW_STATIC -DFREEGLUT_STATIC -std=c++1y -Iinclude -IGraphics/SOIL/src -c "obj_player.cpp" -o obj/Debug/obj_player.o
g++.exe -Wall -std=c++1y -m32 -g -DGLEW_STATIC -DFREEGLUT_STATIC -std=c++1y -Iinclude -IGraphics/SOIL/src -c "obj_wall.cpp" -o obj/Debug/obj_wall.o
g++.exe -Wall -std=c++1y -m32 -g -DGLEW_STATIC -DFREEGLUT_STATIC -std=c++1y -Iinclude -IGraphics/SOIL/src -c "Objects.cpp" -o obj/Debug/Objects.o
g++.exe -Llib-mingw -LGraphics/SOIL/lib -o bin/Debug/NGE.exe obj/Debug/Assets/Input.o obj/Debug/bitwise.o obj/Debug/Engine.o obj/Debug/gmath.o obj/Debug/Graphics/Sprite.o obj/Debug/Graphics/Texture.o obj/Debug/main.o obj/Debug/ngetype.o obj/Debug/obj_player.o obj/Debug/obj_wall.o obj/Debug/Objects.o  -m32 -lftgl_static_D -lfreetype -lfreeglut_static lib-mingw/libglew32s.lib -lglfw3 -lSOIL -lgdi32 -lwinmm -lopengl32
