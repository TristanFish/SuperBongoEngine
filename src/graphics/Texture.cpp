#include "Texture.h"
#include <iostream>

Texture::Texture()
{
	textureID = 0;
	path = "NO PATH SUPPLIED";
}

Texture::Texture(const std::string& path, const std::string& type)
{
	textureID = 0;
	this->path = path;
	this->type = type;
	isLoaded = false;
}

bool Texture::LoadImage()
{
	if (isLoaded)
	{
		return true;
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	SDL_Surface* textureSurface = IMG_Load(path.c_str());
	if (textureSurface == nullptr) {
		std::cout << "texture: " << path << " didn't load" << std::endl;
		return false;
	}
	int numOfColours = textureSurface->format->BytesPerPixel;
	int format;
	if (numOfColours == 4)
	{
		if (textureSurface->format->Rmask == 0x000000ff)
		{
			format = GL_RGBA;
		}
		else
		{
			format = GL_BGRA;
		}
	}
	else if (numOfColours == 3)
	{
		if (textureSurface->format->Rmask == 0x000000ff)
		{
			format = GL_RGB;
		}
		else
		{
			format = GL_BGR;
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, numOfColours, textureSurface->w, textureSurface->h, 0, format, GL_UNSIGNED_BYTE, textureSurface->pixels);
	
	SDL_FreeSurface(textureSurface);
	textureSurface = nullptr;
	/// Wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0); /// Unbind the texture
	isLoaded = true;
	return true;
}

void Texture::SetType(const std::string& typeName)
{
	type = typeName;
}

void Texture::DestroyTexture()
{
	glDeleteTextures(1, &textureID);
}

Texture::~Texture()
{

}
