#include "Sprite.h"
/// Constructors
Sprite::Sprite(){
	xPos = 0;
	yPos = 0;
	xscale=1;
	yscale=1;
	angle = 0;
	texture = Texture();
}
Sprite::Sprite(std::string _imagePath){
    imagePath=_imagePath.c_str();
	texture = Texture(_imagePath);
	xPos = 0;
	yPos = 0;
	xscale=1;
	yscale=1;
	angle = 0;
}
Sprite::Sprite(std::string _imagePath, float _xPos, float _yPos){
    imagePath=_imagePath.c_str();
	texture = Texture(_imagePath);
	xPos = _xPos;
	yPos = _yPos;
	xscale=1;
	yscale=1;
	angle = 0;
}
Sprite::Sprite(std::string _imagePath, float _xPos, float _yPos,float _xScale, float _yScale){
    imagePath=_imagePath.c_str();
    texture = Texture(_imagePath);
	xPos = _xPos;
	yPos = _yPos;
	xscale=_xScale;
	yscale=_yScale;
	angle = 0;
}

void Sprite::Update(){
}
/// Renders Sprite to the screen
void Sprite::Render(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.sprite_get_index());
	glLoadIdentity();
	// > TRANSLATE -> ROTATE -> SCALE
	//GL Translate
	glTranslatef(xPos, yPos, 0);
    glRotatef(angle,0,0,1);
    glScalef(xscale,yscale,1);
	//Render

	glBegin(GL_QUADS);


    glTexCoord2f(1, 0); glVertex2f(texture.sprite_get_width(), 0);
	glTexCoord2f(1, 1); glVertex2f(texture.sprite_get_width(), texture.sprite_get_height());
	glTexCoord2f(0, 1); glVertex2f(0, texture.sprite_get_height());
	glTexCoord2f(0, 0); glVertex2f(0, 0);

	glEnd();
	glDisable(GL_TEXTURE_2D);

}
/// Get Sprite Texture ID
Texture Sprite::getTexture(){
    return texture;
}
/// Get Sprite Width
int Sprite::sprite_get_width(){
    return texture.sprite_get_width();
}
/// Get Sprite Height
int Sprite::sprite_get_height(){
    return texture.sprite_get_height();
}
/// Draws Sprite at position (x, y)
void Sprite::sprite_set_pos(float _xPos, float _yPos){
    xPos = _xPos;
    yPos = _yPos;
}
/// Get x position of a sprite
int Sprite::sprite_get_x(){
    return xPos;
}
/// Get y position of a sprite
int Sprite::sprite_get_y(){
    return yPos;
}
