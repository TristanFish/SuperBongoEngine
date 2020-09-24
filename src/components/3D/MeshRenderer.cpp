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
		std::cout << "Model " << modelPath << " Loaded" << std::endl;
	}
	else
	{
		std::cout << "Model " << modelPath << " Not Loaded" << std::endl;
	}
}

MeshRenderer::~MeshRenderer()
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].DestroyTextures();
	}
}

void MeshRenderer::CreateShader(const char* vert, const char* frag)
{
	shader.CreateShader(vert, frag);
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
		shader.RunShader();
		shader.TakeInUniformMat4("projectionMatrix", Camera::getInstance()->getProjectionMatrix());
		shader.TakeInUniformMat4("viewMatrix", Camera::getInstance()->getViewMatrix());
		shader.TakeInUniformMat4("modelMatrix", gameobject->GetModelMatrix());
		m.Render(shader);
		glUseProgram(0);
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

	directory = modelPath.substr(0, modelPath.find_last_of('/') + 1);
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

	//Load vertices
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

	//Load indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//Load materials
	aiColor4D col(0.0f, 0.0f, 0.0f, 0.0f);
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		//Get material color
		material->Get(AI_MATKEY_COLOR_DIFFUSE, col);

		//Get diffuse textures
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, "diffuseTex");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		
		//Get Specular textures
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType::aiTextureType_SPECULAR, "specularTex");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	MATH::Vec4 color = Vec4(col.r, col.g, col.b, col.a);



	
	p_min.x = vertices[0].position.x;
	p_min.y = vertices[0].position.y;
	p_min.z = vertices[0].position.z;

	p_max.x = vertices[0].position.x;
	p_max.y = vertices[0].position.y;
	p_max.z = vertices[0].position.z;
	// Loops Through verticies and gives us the min and max verticies and put's them into a vector
	for (int i = 1; i < vertices.size(); i++)
	{
		p_min.x = std::min(p_min.x, vertices[i].position.x);
		p_min.y = std::min(p_min.y, vertices[i].position.y);
		p_min.z = std::min(p_min.z, vertices[i].position.z);

		p_max.x = std::max(p_max.x, vertices[i].position.x);
		p_max.y = std::max(p_max.y, vertices[i].position.y);
		p_max.z = std::max(p_max.z, vertices[i].position.z);
	}


	return Mesh(vertices, indices, textures, color);
}

std::vector<Texture> MeshRenderer::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	
	//load all textures of a TextureType (specular, diffuse, etc.) from the selected material
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString path;
		aiString fullPath = aiString(directory);
		Texture texture;

		//Get the path to the texture associated with the model
		mat->GetTexture(type, i, &path);
		//add path to the directory where the model is stored
		fullPath.Append(path.C_Str());

		//Load texture
		texture.LoadImage(fullPath.C_Str());
		texture.type = typeName;
		textures.push_back(texture);
	}
	return textures;
}
