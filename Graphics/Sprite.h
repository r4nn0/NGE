#ifndef GAME_SPRITE
#define GAME_SPRITE

#include "Texture.h"

class Sprite{
public:

	Sprite();
	Sprite(std::string imagePath);
	Sprite(std::string imagePath, float _xPos, float _yPos);
	Sprite(std::string imagePath, float _xPos, float _yPos,float _xScale, float _yScale);
	void Update();
	void Render();
	void sprite_set_pos(float _xPos, float _yPos);
	int sprite_get_width();
	int sprite_get_height();
	int sprite_get_x();
	int sprite_get_y();
	float angle;
	float xscale;
	float yscale;
	Texture getTexture();
	const char* imagePath;
private:
	Texture texture;
	float xPos;
	float yPos;
};

#endif
