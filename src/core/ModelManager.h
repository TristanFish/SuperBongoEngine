#pragma once
#include <unordered_map>
#include "components/3D/MeshRenderer.h"


class ModelManager
{
public:
	static std::vector<Mesh>  GetModel(const std::string& name);
private:
	static void LoadAllModels();

	static std::unordered_map<std::string, std::vector<Mesh>> models;

	friend class GameManager;
};



