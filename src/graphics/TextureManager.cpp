#include "TextureManager.h"
#include <iostream>

TextureManager::TextureManager()
{
	textureID = 0;
}



bool TextureManager::LoadImage(const char* filename)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	SDL_Surface* textureSurface = IMG_Load(filename);
	if (textureSurface == nullptr) {
		std::cout << "texture: " << filename << " didn't load" << std::endl;
		return false;
	}
	int mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	
	SDL_FreeSurface(textureSurface);
	textureSurface = nullptr;
	/// Wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0); /// Unbind the texture
	return false;
}


TextureManager::~TextureManager()
{
	glDeleteTextures(1, &textureID);
}
