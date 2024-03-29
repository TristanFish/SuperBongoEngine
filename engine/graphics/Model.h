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

	std::vector<Vertex> Model_Verticies;

	std::string modelPath;

	Model();
	Model(const std::string& path);

	const MATH::Vec3 GetColor() { return MATH::Vec3(meshes[0].color); }
	void SetColor(const MATH::Vec3 color_) { meshes[0].color = MATH::Vec4(color_.x,color_.y,color_.z, meshes[0].color.w); }

	std::vector<Vertex>& GetVerticies();


	void LoadModel();
	void DestroyModel();
	std::vector<Vertex> GetVertices() const;

	
 private:


	 void ProcessNode(aiNode* node, const aiScene* scene);
	 Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	 std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);


};

#endif