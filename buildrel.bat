g++.exe -Wall -DGLEW_STATIC -O2 -Iinclude -Iinclude -c Engine.cpp -o obj\Release\Engine.o
g++.exe -Wall -DGLEW_STATIC -O2 -Iinclude -Iinclude -c gmath.cpp -o obj\Release\gmath.o
g++.exe -Wall -DGLEW_STATIC -O2 -Iinclude -Iinclude -c Graphics\Renderer2D.cpp -o obj\Release\Graphics\Renderer2D.o
g++.exe -Wall -DGLEW_STATIC -O2 -Iinclude -Iinclude -c Graphics\Sprite.cpp -o obj\Release\Graphics\Sprite.o
g++.exe -Wall -DGLEW_STATIC -O2 -Iinclude -Iinclude -c Graphics\stb_image.cpp -o obj\Release\Graphics\stb_image.o
g++.exe -Wall -DGLEW_STATIC -O2 -Iinclude -Iinclude -c Input.cpp -o obj\Release\Input.o
g++.exe -Wall -DGLEW_STATIC -O2 -Iinclude -Iinclude -c main.cpp -o obj\Release\main.o
g++.exe -Wall -DGLEW_STATIC -O2 -Iinclude -Iinclude -c ngetype.cpp -o obj\Release\ngetype.o
g++.exe -Llib -o bin\Release\NGE.exe obj\Release\Engine.o obj\Release\gmath.o obj\Release\Graphics\Renderer2D.o obj\Release\Graphics\Sprite.o obj\Release\Graphics\stb_image.o obj\Release\Input.o obj\Release\main.o obj\Release\ngetype.o  -s  -lftgl_static -lfreetype lib\libglew32s.lib -lglfw3 -lgdi32 -lwinmm -lopengl32
