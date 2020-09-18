#pragma once
#include "sdl/SDL.h"
#include "sdl/SDL_image.h"
#include <glew/glew.h>
#include <string>

class Texture
{
	GLuint textureID;

public:
	std::string type;

	Texture();
	~Texture();
	bool LoadImage(const char* filename);
	
	inline GLuint getTextureID() const { return textureID; }

};