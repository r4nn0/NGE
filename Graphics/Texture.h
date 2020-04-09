#ifndef GAME_TEXTURE
#define GAME_TEXTURE

#include <iostream>

#include "GLFW/glfw3.h"
#include "SOIL/src/SOIL.h"

using namespace std;
class Texture{
public:
	Texture();
	Texture(int _id);
	Texture(std::string path);
	~Texture();
	int sprite_get_index();
	int sprite_get_width();
	int sprite_get_height();
private:
	bool GetTextureParams();
	int sprite_index;
	int sprite_width;
	int sprite_height;
};
#endif
