#include "ModelManager.h"
#include "TextureManager.h"
#include <filesystem>
#include <iostream>
#include <cassert>

std::unordered_map<std::string, Model> ModelManager::models;

void ModelManager::LoadAllModels()
{
	using namespace std::filesystem;
	std::cout << "Loading all models" << std::endl;
	for (auto& folder : std::filesystem::recursive_directory_iterator("resources/models"))
	{
		if (folder.path().filename().extension() == ".mtl")
		{
			continue;
		}
		models[folder.path().filename().string()] = Model(relative(folder.path()).string());
		std::cout << folder.path().filename().string() << std::endl;
	}
}

Model& ModelManager::GetModel(const std::string& name)
{
	if (models.find(name) == models.end())
	{
		#undef NDEBUG
		std::cerr << "No model found with the name: " << name << std::endl;
		assert(false && "Closing program");
	}

	if (!models[name].isLoaded)
	{
		models[name].LoadModel();
		return models[name];
	}
	return models[name];
}

void ModelManager::DestroyAllModels()
{
	for (auto& pair : models)
	{
		if (pair.second.isLoaded)
		{
			pair.second.DestroyModel();
		}
	}
	models.clear();
}
