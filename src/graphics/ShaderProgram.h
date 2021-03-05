#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <vector>
#include <string>
#include "math/Matrix.h"
#include "glew/glew.h"

class ShaderProgram
{
	friend class ShaderManager;
	
	GLuint programID;
	GLuint LinkShaders(const std::vector<GLint>& shaders);

public:

	ShaderProgram();
	bool operator==(const ShaderProgram& sp) const;
	
	void DeleteProgram();

	
	void RunShader() const;
	
	void TakeUniform(const std::string &name, const MATH::Matrix4 &matrix) const;
	void TakeUniform(const std::string &name, const MATH::Matrix3 &matrix) const;
	void TakeUniform(const std::string &name, const MATH::Vec3& v) const;
	void TakeUniform(const std::string &name, const MATH::Vec4& v) const;
	void TakeUniform(const std::string &name, const float f) const;
	void TakeUniform(const std::string &name, const int i) const;

	GLuint GetID() const { return programID; }
};

#endif