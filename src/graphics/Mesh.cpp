#include "Mesh.h"
#include "core/ShaderManager.h"

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

void Mesh::RenderRegular(const ShaderProgram& shader) const
{
	size_t diffN = 1;
	size_t specN = 1;


	//For every texture on the mesh bind it to the shader
	for (size_t i = 0; i < textures.size(); i++)
	{
		//setting up a naming scheme for textures inside of shaders, check DefaultFrag.glsl
		//texture names should be "typeTexNumber" e.g.(diffuseTex1)
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = textures[i].type;
		if (name == "diffuseTex")
		{
			number = std::to_string(diffN++);
		}
		else if (name == "specularTex")
		{
			number = std::to_string(specN++);
		}

		glUniform1i(glGetUniformLocation(shader.GetID(), (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].getTextureID());
	}
	glActiveTexture(GL_TEXTURE0);
	
	if (diffN <= 1)
	{
		//if there are no diffuse textures, add the color of the material to the shader
		glUniform4fv(glGetUniformLocation(shader.GetID(), "meshColor"), 1, color);
	}
	else
	{
		glUniform4fv(glGetUniformLocation(shader.GetID(), "meshColor"), 1, Vec4());
	}

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (size_t i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

// Used for instance rendering 
void Mesh::RenderInstanced(const ShaderProgram& shader, const std::vector<Mesh>& meshes,const unsigned int amount) const
{
	unsigned int diffN = 1;
	unsigned int specN = 1;


	//For every texture on the mesh bind it to the shader
	//This loop will probably start to get slow as we add more objects
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		//setting up a naming scheme for textures inside of shaders, check DefaultFrag.glsl
		//texture names should be "typeTexNumber" e.g.(diffuseTex1)
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = textures[i].type;
		if (name == "diffuseTex")
		{
			number = std::to_string(diffN++);
		}
		else if (name == "specularTex")
		{
			number = std::to_string(specN++);
		}

		glUniform1i(glGetUniformLocation(shader.GetID(), (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].getTextureID());
	}
	glActiveTexture(GL_TEXTURE0);

	if (diffN == 1)
	{
		//if there are no diffuse textures, add the color of the material to the shader
		glUniform4fv(glGetUniformLocation(shader.GetID(), "meshColor"), 1, color);
	}

	
	glBindVertexArray(meshes[0].GetVAO());
	glDrawElementsInstanced(GL_TRIANGLES, meshes[0].indices.size(), GL_UNSIGNED_INT, 0, amount);
	glBindVertexArray(0);

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


