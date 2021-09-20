g++.exe -Wall -DGLEW_STATIC -g -Iinclude -c Engine.cpp -o obj\Debug\Engine.o
g++.exe -Wall -DGLEW_STATIC -g -Iinclude -c gmath.cpp -o obj\Debug\gmath.o
g++.exe -Wall -DGLEW_STATIC -g -Iinclude -c Graphics\Renderer2D.cpp -o obj\Debug\Graphics\Renderer2D.o
g++.exe -Wall -DGLEW_STATIC -g -Iinclude -c Graphics\Sprite.cpp -o obj\Debug\Graphics\Sprite.o
g++.exe -Wall -DGLEW_STATIC -g -Iinclude -c Graphics\TexturePage.cpp -o obj\Debug\Graphics\TexturePage.o
g++.exe -Wall -DGLEW_STATIC -g -Iinclude -c Graphics\stb_image.cpp -o obj\Debug\Graphics\stb_image.o
g++.exe -Wall -DGLEW_STATIC -g -Iinclude -c Input.cpp -o obj\Debug\Input.o
g++.exe -Wall -DGLEW_STATIC -g -Iinclude -c main.cpp -o obj\Debug\main.o
g++.exe -Wall -DGLEW_STATIC -g -Iinclude -c ngetype.cpp -o obj\Debug\ngetype.o
g++.exe -Llib -o bin\Debug\NGE.exe obj\Debug\Engine.o obj\Debug\gmath.o obj\Debug\Graphics\Renderer2D.o obj\Debug\Graphics\Sprite.o obj\Debug\Graphics\TexturePage.o obj\Debug\Graphics\stb_image.o obj\Debug\Input.o obj\Debug\main.o obj\Debug\ngetype.o -lftgl_static -lfreetype -lglew32s -lglfw3 -lgdi32 -lwinmm -lopengl32
