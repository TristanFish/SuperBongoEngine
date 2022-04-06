#include "Mesh.h"
#include "core/resources/ShaderManager.h"

using namespace MATH;

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures, MATH::Vec4 color)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->color = color;

}

Mesh::Mesh(const Mesh& m)
{
	vertices = m.vertices;
	indices = m.indices;
	textures = m.textures;
	color = m.color;
	shader = m.shader;
	vao = m.vao;
	vbo = m.vbo;
	ebo = m.ebo;
}

Mesh::~Mesh()
{
	
}

void Mesh::DeleteMesh()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

Mesh& Mesh::operator=(const Mesh& m)
{
	vertices = m.vertices;
	indices = m.indices;
	textures = m.textures;
	color = m.color;
	shader = m.shader;
	vao = m.vao;
	vbo = m.vbo;
	ebo = m.ebo;
	return *this;
}

void Mesh::InitMesh()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));

	glBindVertexArray(0);
}


