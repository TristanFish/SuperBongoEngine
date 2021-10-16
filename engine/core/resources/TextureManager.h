#pragma once
#include <unordered_map>
#include "graphics/Texture.h"


class TextureManager
{
public:
	static Texture& GetTexture(const std::string& name);
	static void DeleteAllTextures();

private:
	static void LoadAllTextures();

	static std::unordered_map<std::string, Texture> textures;
	
	friend class CoreEngine;
};


