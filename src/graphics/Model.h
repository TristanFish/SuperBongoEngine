#pragma once
#include "graphics/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public: 
	std::vector<Mesh> meshes;

	bool isLoaded;

	Vec3 p_min;
	Vec3 p_max;

	std::string modelPath;

	Model();
	Model(const std::string& path);

	const Vec3 GetColor() { return Vec3(meshes[0].color); }
	void SetColor(const Vec3 color_) { meshes[0].color = Vec4(color_.x,color_.y,color_.z, meshes[0].color.w); }

	void LoadModel();
	void DestroyModel();
	
private:
	Vec4 color;

	 void ProcessNode(aiNode* node, const aiScene* scene);
	 Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	 std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
};

