#ifndef SHADER_H
#define SHADER_H

#include "sdl/SDL.h"
#include "glew/glew.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include "Matrix.h"

using namespace MATH;
class Shader
{
public:
	GLuint CreateShader(const char* vertexPath, const char* fragmentPath);
	void RunShader() const;
	Matrix4 viewProjectionSize;
	void TakeInUniformMat4(const std::string &name, Matrix4 &matrix) const;
	void TakeInUniformVec3(const std::string& name,  const Vec3& value) const;
	void TakeInUniformFloat(const std::string& name, const float& value) const;
	inline GLuint GetID() const { return shader_program; }
	inline void SetID(GLuint id) { shader_program = id; }

private:
	GLuint shader_program;
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
};
#endif // !SHADER_H

