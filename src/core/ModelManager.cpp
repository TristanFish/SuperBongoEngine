#include "ModelManager.h"

std::unordered_map<std::string, std::vector<Mesh>> ModelManager::models;

void ModelManager::LoadAllModels()
{
	
}

std::vector<Mesh> ModelManager::GetModel(const std::string& name)
{
	return models[name];
}