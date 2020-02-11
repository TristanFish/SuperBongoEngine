#include "TextureManager.h"

/*SDL_Texture* TextureManager::loadTexture(const char* fileName)
{
}
//This needs to be done a differant way for it to work with our code
/*void TextureManager::Render(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest)
{
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL);
}*/

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
		return false;
	}
	int mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
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
