#pragma once
#include <unordered_map>
#include "graphics/Model.h"

class ModelManager
{
public:
	static Model&  GetModel(const std::string& name);
	static void DestroyAllModels();
private:
	static std::unordered_map<std::string, Model> models;

	static void LoadAllModels();

	friend class GameManager;
};



