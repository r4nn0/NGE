#include "Texture.h"

Texture::Texture(){
	sprite_index = -1;
}
Texture::Texture(int _id){
	sprite_index = _id;
	if (!GetTextureParams()){
		cout << "Error Loading Image With ID: " << sprite_index << endl;
	}
}
Texture::Texture(std::string path){
	sprite_index = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MULTIPLY_ALPHA);
	if (!GetTextureParams()){
		cout << "Error Loading Image: "<< path << endl;
	}

}
Texture::~Texture(){}
int Texture::sprite_get_index(){
	return sprite_index;
}
int Texture::sprite_get_width(){
	return sprite_width;
}
int Texture::sprite_get_height(){
	return sprite_height;
}
bool Texture::GetTextureParams(){
	if (sprite_index > 0){
		glBindTexture(GL_TEXTURE_2D, sprite_index);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &sprite_width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &sprite_height);
		return true;
	}
	return false;
}
