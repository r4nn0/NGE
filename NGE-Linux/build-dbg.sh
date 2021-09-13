#!/bin/bash
g++ -Wall -g -Iinclude -Iinclude -c Engine.cpp -o obj/Debug/Engine.o
g++ -Wall -g -Iinclude -Iinclude -c gmath.cpp -o obj/Debug/gmath.o
g++ -Wall -g -Iinclude -Iinclude -c Graphics/Renderer2D.cpp -o obj/Debug/Graphics/Renderer2D.o
g++ -Wall -g -Iinclude -Iinclude -c Graphics/Sprite.cpp -o obj/Debug/Graphics/Sprite.o
g++ -Wall -g -Iinclude -Iinclude -c Graphics/stb_image.cpp -o obj/Debug/Graphics/stb_image.o
g++ -Wall -g -Iinclude -Iinclude -c Input.cpp -o obj/Debug/Input.o
g++ -Wall -g -Iinclude -Iinclude -c main.cpp -o obj/Debug/main.o
g++ -Wall -g -Iinclude -Iinclude -c ngetype.cpp -o obj/Debug/ngetype.o
g++ -o bin/NGE-Debug.bin obj/Debug/Engine.o obj/Debug/gmath.o obj/Debug/Graphics/Renderer2D.o obj/Debug/Graphics/Sprite.o obj/Debug/Graphics/stb_image.o obj/Debug/Input.o obj/Debug/main.o obj/Debug/ngetype.o -lftgl -lfreetype -lGL -lGLEW -lglfw
