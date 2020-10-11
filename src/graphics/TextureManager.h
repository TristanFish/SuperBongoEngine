#pragma once
#include "sdl/SDL.h"
#include "sdl/SDL_image.h"
#include <glew/glew.h>

class TextureManager
{
	GLuint textureID;

public:
	TextureManager();
	~TextureManager();
	bool LoadImage(const char* filename);
	//This needs to be done a differant way for it to work with our code 
	inline GLuint getTextureID() const { return textureID; }

};