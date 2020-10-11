#include "TextureManager.h"

std::unordered_map<std::string, Texture> TextureManager::textures;

void TextureManager::LoadAllTextures()
{

}

Texture TextureManager::GetTexture(const std::string& name)
{
	return textures[name];
}