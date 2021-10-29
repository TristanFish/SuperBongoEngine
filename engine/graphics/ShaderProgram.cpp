#include "ShaderProgram.h"

#include "core/Logger.h"

ShaderProgram::ShaderProgram()
{
	programID = 0;
}

bool ShaderProgram::operator==(const ShaderProgram& sp) const
{
	return programID == sp.programID;
}

void ShaderProgram::DeleteProgram()
{
	glDeleteProgram(programID);
}


GLuint ShaderProgram::LinkShaders(const std::vector<GLint>& shaders)
{
	programID = glCreateProgram();
	if (programID == 0)
	{
		EngineLogger::Error("Shader not loaded", "Shader.cpp", __LINE__);
	}

	//link every shader to this program
	for(size_t i = 0; i < shaders.size(); i++)
	{
		glAttachShader(programID, shaders[i]);
	}

	GLint status;
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if(status == 0)
	{
		GLsizei errorLogSize = 0;
		std::string errorLog;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &errorLogSize);
		errorLog.resize(errorLogSize);
		glGetProgramInfoLog(programID, errorLogSize, &errorLogSize, &errorLog[0]);
		EngineLogger::Error(errorLog, "ShaderProgram.cpp", __LINE__);
	}
	
	
	return programID;
}

void ShaderProgram::RunShader() const
{
	glUseProgram(programID);
}

void ShaderProgram::TakeUniform(const std::string &name, const MATH::Matrix4 &matrix) const
{
	const GLint location = glGetUniformLocation(programID, name.c_str());
	if (location < 0) 
	{
		EngineLogger::Warning("Uniform ID: " + name + " not found or unused", "Shader.cpp", __LINE__);
		return; 
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
}

void ShaderProgram::TakeUniform(const std::string& name, const MATH::Matrix3& matrix) const
{
	const GLint location = glGetUniformLocation(programID, name.c_str());
	if (location < 0) { 
		EngineLogger::Warning("Uniform ID: " + name + " not found or unused", "Shader.cpp", __LINE__);
		return; 
	}
	glUniformMatrix3fv(location, 1, GL_FALSE, matrix);
}

void ShaderProgram::TakeUniform(const std::string& name, const MATH::Vec3& v) const
{
	const GLint location = glGetUniformLocation(programID, name.c_str());
	if (location < 0) 
	{ 
		EngineLogger::Warning("Uniform ID: " + name + " not found or unused", "Shader.cpp", __LINE__);
		return; 
	}
	glUniform3f(location, v.x, v.y, v.z);
}

void ShaderProgram::TakeUniform(const std::string& name, const MATH::Vec4& v) const
{
	const GLint location = glGetUniformLocation(programID, name.c_str());
	if (location < 0)
	{ 
		EngineLogger::Warning("Uniform ID: " + name + " not found or unused", "Shader.cpp", __LINE__); 
		return; 
	}
	glUniform4fv(location, 1, v);
}

void ShaderProgram::TakeUniform(const std::string& name, const float f) const
{
	const GLint location = glGetUniformLocation(programID, name.c_str());
	if (location < 0) 
	{
		EngineLogger::Warning("Uniform ID: " + name + " not found or unused", "Shader.cpp", __LINE__); 
		return; 
	}
	glUniform1f(location, f);
}

void ShaderProgram::TakeUniform(const std::string& name, const int i) const
{
	const GLint location = glGetUniformLocation(programID, name.c_str());
	if (location < 0) 
	{ 
		EngineLogger::Warning("Uniform ID: " + name + " not found or unused", "Shader.cpp", __LINE__); 
		return; 
	}
	glUniform1i(location, i);

}

void ShaderProgram::TakeUniform(const std::string& name, const Uint16 i) const
{
	const GLint location = glGetUniformLocation(programID, name.c_str());
	if (location < 0) 
	{ 
		EngineLogger::Warning("Uniform ID: " + name + " not found or unused", "Shader.cpp", __LINE__); 
		return; 
	}
	glUniform1ui(location, i);
}
