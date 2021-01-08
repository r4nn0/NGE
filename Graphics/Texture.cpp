#include "Texture.h"

Texture::Texture(){
	TextureID = -1;
}
Texture::Texture(int _id){
	TextureID = _id;
	if (!GetTextureParams()){
		std::cout << "Error Loading Image With ID: " << TextureID << std::endl;
	}

}
Texture::Texture(std::string path){
	TextureID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MULTIPLY_ALPHA);
	if (!GetTextureParams()){
		std::cout << "Error Loading Image: "<< path << std::endl;
	}
}
Texture::~Texture(){}
int Texture::TextureGetID(){
	return TextureID;
}
int Texture::TextureGetWidth(){
	return TextureWidth;
}
int Texture::TextureGetHeight(){
	return TextureHeight;
}
bool Texture::GetTextureParams(){
	if (TextureID > 0){
		glBindTexture(GL_TEXTURE_2D, TextureID);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &TextureWidth);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &TextureHeight);
		return true;
	}
	return false;
}
