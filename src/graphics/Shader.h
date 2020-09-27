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
	Matrix4 viewProjectionSize;

	GLuint CreateShader(const char* vertexPath, const char* fragmentPath);
	void RunShader() const;
	
	void TakeInUniformMat4(const std::string &name, Matrix4 &matrix) const;
	void TakeInUniformVec3(const std::string &name, Vec3& v) const;
	void TakeInUniformVec4(const std::string &name, Vec3& v) const;
	void TakeInUniformfloat(const std::string &name, float v) const;
	void TakeInUniformInt(const std::string &name, int v) const;


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

