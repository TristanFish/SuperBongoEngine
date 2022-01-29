#include "TextureManager.h"
#include <filesystem>
#include <iostream>
#include <cassert>
#include "core/Logger.h"

std::unordered_map<std::string, Texture> TextureManager::textures;

void TextureManager::LoadAllTextures()
{
	using namespace std::filesystem;
	EngineLogger::Info("=================Loading all textures=================", "TextureManager.cpp", __LINE__);
	for (const auto& folder : recursive_directory_iterator("resources/textures"))
	{
		textures[folder.path().filename().string()] = Texture(relative(folder.path()).string());
		EngineLogger::Info(folder.path().filename().string(), "TextureManager.cpp", __LINE__);
	}

}

Texture& TextureManager::GetTexture(const std::string& name)
{
	if (textures.find(name) == textures.end())
	{
		EngineLogger::Error("No texture found with the name: " + name, "ModelManager.cpp", __LINE__);

		assert(false && "Closing program");
	}

	if (!textures[name].isLoaded)
	{
		textures[name].LoadImage();
		return textures[name];
	}
	else
	{
		return textures[name];
	}
}

void TextureManager::DeleteAllTextures()
{
	if(textures.empty())
	{
		return;
	}
	
	for (auto& pair : textures)
	{
		
		if (pair.second.isLoaded)
		{
			pair.second.DestroyTexture();
		}
	}
	textures.clear();
}
