#include "TextureManager.h"
#include <filesystem>
#include <iostream>
#include <cassert>

std::unordered_map<std::string, Texture> TextureManager::textures;

void TextureManager::LoadAllTextures()
{
	using namespace std::filesystem;
	std::cout << "Loading all textures" << std::endl;
	for (auto& folder : std::filesystem::recursive_directory_iterator("resources/textures"))
	{
		textures[folder.path().filename().string()] = Texture(relative(folder.path()).string());
		std::cout << folder.path().filename().string() << std::endl;
	}
}

Texture& TextureManager::GetTexture(const std::string& name)
{
	if (textures.find(name) == textures.end())
	{
		std::cerr << "A texture named " << name << " was not found" << std::endl;
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
	for (auto& pair : textures)
	{
		if (pair.second.isLoaded)
		{
			pair.second.DestroyTexture();
		}
	}
	textures.clear();
}
