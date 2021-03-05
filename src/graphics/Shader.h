#ifndef SHADER_H
#define SHADER_H

#include "glew/glew.h"
#include <fstream>

//
class Shader
{
public:
	friend class ShaderManager;
	
	Shader();
	Shader(const char* path);
	Shader(const Shader&);
	Shader& operator =(const Shader&);

	~Shader() = default;

	void DeleteShader();
	
	GLuint CreateShader(const char* shaderPath);

	inline GLuint GetID() const { return shaderID; }

private:
	GLint shaderID;
	std::ifstream shaderFile;
	uint64_t shaderType;
};
#endif // !SHADER_H

