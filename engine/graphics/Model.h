#ifndef MODEL_H
#define MODEL_H


#include "graphics/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>


class Model
{
public: 
	std::vector<Mesh> meshes;

	bool isLoaded;

	MATH::Vec3 p_min;
	MATH::Vec3 p_max;

	std::string modelPath;

	Model();
	Model(const std::string& path);

	void LoadModel();
	void DestroyModel();
	
private:

	 void ProcessNode(aiNode* node, const aiScene* scene);
	 Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	 std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
};

#endif