#ifndef MESH_H
#define MESH_H

#include <vector>
#include "graphics/Vertex.h"
#include "graphics/Texture.h"
#include "graphics/ShaderProgram.h"

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	MATH::Vec4 color;
	ShaderProgram shader;

	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures, MATH::Vec4 color);
	Mesh(const Mesh& m);
	~Mesh();

	void InitMesh();
	void DeleteMesh();

	Mesh& operator = (const Mesh& m);
	
	void RenderRegular(const ShaderProgram& shader) const;
	void RenderInstanced(const ShaderProgram& shader, const std::vector<Mesh>& meshes ,const unsigned int amount) const;
	GLuint GetVAO() const { return vao; }
private:
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

};
#endif
