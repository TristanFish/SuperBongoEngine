#ifndef TEXTURE_H
#define TEXTURE_H


#include <glew/glew.h>
#include <string>

class Texture
{
	GLuint textureID;

public:
	std::string type;
	std::string path;
	bool isLoaded;

	Texture();
	Texture(const std::string& path, const std::string& type = "noType");
	~Texture();
	bool LoadImage();
	void SetType(const std::string& typeName);
	void DestroyTexture();
	inline GLuint getTextureID() const { return textureID; }

};

#endif