#include <assimp/Importer.hpp>
#include "MeshRenderer.h"
#include "custom/Camera.h"

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::MeshRenderer(const char* modelPath)
{
	if(LoadModel(modelPath))
	{
		std::cout << "Model Loaded" << std::endl;
	}
	else
	{
		std::cout << "Model Not Loaded" << std::endl;
	}
}

MeshRenderer::~MeshRenderer()
{
	if (shader) { delete shader; }
}

void MeshRenderer::CreateShader(const char* vert, const char* frag)
{
	shader = new Shader();
	shader->CreateShader(vert, frag);
}

void MeshRenderer::Init(GameObject* g)
{
	gameobject = g;
}

void MeshRenderer::Update(const float deltaTime)
{
}

void MeshRenderer::Render() const
{
	for (auto& m : meshes)
	{
		shader->RunShader();
		shader->TakeInUniformMat4("projectionMatrix", Camera::getInstance()->getProjectionMatrix());
		shader->TakeInUniformMat4("viewMatrix", Camera::getInstance()->getViewMatrix());
		shader->TakeInUniformMat4("modelMatrix", gameobject->GetModelMatrix());
		m.Render(*shader);
	}
}

void MeshRenderer::HandleEvents(const SDL_Event& event)
{
}

bool MeshRenderer::LoadModel(std::string modelPath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ASSIMP error: " << importer.GetErrorString() << std::endl;
		return false;
	}

	directory = modelPath.substr(0, modelPath.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
	return true;
}

void MeshRenderer::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh MeshRenderer::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		
		MATH::Vec3 vec;
		vec.x = mesh->mVertices[i].x;
		vec.y = mesh->mVertices[i].y;
		vec.z = mesh->mVertices[i].z;
		vertex.position = vec;
		vec.x = mesh->mNormals[i].x;
		vec.y = mesh->mNormals[i].y;
		vec.z = mesh->mNormals[i].z;
		vertex.normal = vec;

		if (mesh->mTextureCoords[0])
		{
			MATH::Vec2 uv;
			uv.x = mesh->mTextureCoords[0][i].x;
			uv.y = mesh->mTextureCoords[0][i].y;
			vertex.uv = uv;
		}
		else
		{
			vertex.uv = MATH::Vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType::aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> MeshRenderer::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		texture.LoadImage("resources/textures/pufflet.bmp");
		texture.type = typeName;
		textures.push_back(texture);
	}
	return textures;
}
