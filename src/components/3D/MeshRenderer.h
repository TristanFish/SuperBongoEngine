#pragma once
#include "graphics/Mesh.h"
#include "graphics/Shader.h"
#include "components/ECS.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class MeshRenderer : public Component
{
public:

	MeshRenderer();
	MeshRenderer(const char* modelPath);
	virtual ~MeshRenderer();

	void CreateShader(const char* vert, const char* frag);
	// Inherited via Component
	void Init(GameObject* g) override;
	void Update(const float deltaTime) override;
	void Render() const override;
	void HandleEvents(const SDL_Event& event) override;

private:
	std::vector<Mesh> meshes;
	std::string directory;
	Shader shader;

	bool LoadModel(std::string modelPath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};