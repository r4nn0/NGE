#include "Sprite.h"
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
Texture Sprite::getTexture(){ return texture;}
int Sprite::sprite_get_width(){
    return texture.sprite_get_width();
}
int Sprite::sprite_get_height(){
    return texture.sprite_get_height();
}
void Sprite::sprite_set_pos(float _xPos, float _yPos){
    xPos = _xPos;
    yPos = _yPos;
}
int Sprite::sprite_get_x(){
    return xPos;
}
int Sprite::sprite_get_y(){
    return yPos;
}
