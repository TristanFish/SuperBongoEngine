#pragma once
#include <unordered_map>
#include <memory>
#include "graphics/Texture.h"


class TextureManager
{
public:
	 Texture& GetTexture(const std::string& name);
	 void DeleteAllTextures();

	 static TextureManager* GetInstance();

	 TextureManager();
	 ~TextureManager();

private:

	static std::unique_ptr<TextureManager> textureManagerInstance;
	friend std::default_delete<TextureManager>;

	 void LoadAllTextures();

	 std::unordered_map<std::string, Texture> textures;
	
	friend class CoreEngine;
};


