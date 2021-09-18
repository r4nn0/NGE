#!/bin/bash
g++ -Wall -O2 -c Engine.cpp -o obj/Release/Engine.o
g++ -Wall -O2 -c gmath.cpp -o obj/Release/gmath.o
g++ -Wall -O2 -c Graphics/Renderer2D.cpp -o obj/Release/Graphics/Renderer2D.o
g++ -Wall -O2 -c Graphics/Sprite.cpp -o obj/Release/Graphics/Sprite.o
g++ -Wall -O2 -c Graphics/TexturePage.cpp -o obj/Release/Graphics/TexturePage.o
g++ -Wall -O2 -c Graphics/stb_image.cpp -o obj/Release/Graphics/stb_image.o
g++ -Wall -O2 -c Input.cpp -o obj/Release/Input.o
g++ -Wall -O2 -c main.cpp -o obj/Release/main.o
g++ -Wall -O2 -c ngetype.cpp -o obj/Release/ngetype.o
g++ -o bin/NGE.bin obj/Release/Engine.o obj/Release/gmath.o obj/Release/Graphics/Renderer2D.o obj/Release/Graphics/Sprite.o obj/Release/Graphics/TexturePage.o obj/Release/Graphics/stb_image.o obj/Release/Input.o obj/Release/main.o obj/Release/ngetype.o -s -lftgl -lfreetype -lGL -lGLEW -lglfw
