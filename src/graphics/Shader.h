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
	

private:
	GLuint shader_program;
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
};
#endif // !SHADER_H

