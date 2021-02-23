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
	
	void TakeUniform(const std::string &name, const Matrix4 &matrix) const;
	void TakeUniform(const std::string &name, const Matrix3 &matrix) const;
	void TakeUniform(const std::string &name, const Vec3& v) const;
	void TakeUniform(const std::string &name, const Vec4& v) const;
	void TakeUniform(const std::string &name, const float f) const;
	void TakeUniform(const std::string &name, const int i) const;



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

