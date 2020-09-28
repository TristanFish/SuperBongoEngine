#pragma once
#include <vector>
#include "graphics/Vertex.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	MATH::Vec4 color;
	Shader shader;

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, MATH::Vec4 color);
	Mesh(const Mesh& m);
	~Mesh();
	void DestroyTextures();
	void RenderRegular(const Shader& shader) const;
	void RenderInstanced(const Shader& shader, std::vector<Mesh> meshes ,const unsigned int amount) const;
	GLuint GetVAO() const { return vao; }
private:
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	void InitMesh();
};

