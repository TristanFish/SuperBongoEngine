#pragma once
#include "graphics/Mesh.h"
#include "graphics/Shader.h"
#include "components/ECS.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


enum class RenderProperties : char
{
	NONE				= 0b00000000,
	LIGHTING			= 0b00000001,
	CREATES_SHADOWS		= 0b00000010,
	RECIEVES_SHADOWS	= 0b00000100,
	BLOOM				= 0b00001000,
	PHYSICS_MOVEMENT	= 0b00010000

};

inline constexpr char operator&(RenderProperties rp1, RenderProperties rp2)
{
	return (static_cast<char>(rp1) & static_cast<char>(rp2));
}


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
	void Render(const Shader& shader) const;
	void HandleEvents(const SDL_Event& event) override;

	RenderProperties renderFlags;

	// Used For Axis Aligned Binding Boxes
	Vec3 p_min;
	Vec3 p_max;
private:
	std::vector<Mesh> meshes;
	std::string directory;
	Shader shader;

	bool LoadModel(std::string modelPath);


	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};