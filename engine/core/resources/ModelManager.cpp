#include "ModelManager.h"
#include "TextureManager.h"
#include <filesystem>
#include <cassert>
#include "core/Logger.h"

std::unordered_map<std::string, Model> ModelManager::models;

void ModelManager::LoadAllModels()
{
	using namespace std::filesystem;
	EngineLogger::Info("=================Loading all models=================", "ModelManager.cpp", __LINE__);

	for (const auto& folder : std::filesystem::recursive_directory_iterator("resources/models"))
	{
		if (folder.path().filename().extension() == ".mtl")
		{
			continue;
		}
		models[folder.path().filename().string()] = Model(relative(folder.path()).string());

		EngineLogger::Info(folder.path().filename().string(), "ModelManager.cpp", __LINE__);
	}
}

Model& ModelManager::GetModel(const std::string& name)
{
	if (models.find(name) == models.end())
	{
		EngineLogger::Error("No model found with the name: " + name, "ModelManager.cpp", __LINE__);
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
