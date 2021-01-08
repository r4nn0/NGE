#ifndef GAME_TEXTURE
#define GAME_TEXTURE

#include <iostream>
#include "../Engine.h"
#include "GLFW/glfw3.h"
#include "SOIL/src/SOIL.h"

class Texture{
public:
	Texture();
	Texture(int _id);
	Texture(std::string path);
	~Texture();
	int TextureGetID();
	int TextureGetWidth();
	int TextureGetHeight();
private:
	bool GetTextureParams();
	int TextureID;
	int TextureWidth;
	int TextureHeight;
};
#endif
