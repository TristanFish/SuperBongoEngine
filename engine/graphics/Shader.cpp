#include "Shader.h"
#include "core/Logger.h"
#include <sstream>


Shader::Shader()
{
	shaderID = 0;
}

Shader::Shader(const char* path)
{
	CreateShader(path);
}

Shader::Shader(const Shader& s)
{
	*this = s;
}

Shader& Shader::operator=(const Shader& s)
{
	shaderID = s.shaderID;
	shaderType = s.shaderType;

	return *this;
}


void Shader::DeleteShader()
{
	glDeleteShader(shaderID);
}

GLuint Shader::CreateShader(const char* shaderPath)
{
	// open files
	shaderFile.open(shaderPath);
	if(shaderFile.fail())
	{
		EngineLogger::Warning("Shader " + std::string(shaderPath) + " could not be opened", "Shader.cpp", __LINE__);
	}


	shaderType = 0;
	
	std::string stringPath = shaderPath;

	if(stringPath.find("Vert.glsl") != std::string::npos)
	{
		shaderType = GL_VERTEX_SHADER;
	}
	else if(stringPath.find("TessC.glsl") != std::string::npos)
	{
		shaderType = GL_TESS_CONTROL_SHADER;
	}
	else if(stringPath.find("TessE.glsl") != std::string::npos)
	{
		shaderType = GL_TESS_EVALUATION_SHADER;
	}
	else if(stringPath.find("Geom.glsl") != std::string::npos)
	{
		shaderType = GL_GEOMETRY_SHADER;
	}
	else if(stringPath.find("Frag.glsl") != std::string::npos)
	{
		shaderType = GL_FRAGMENT_SHADER;
	}
	else
	{
		EngineLogger::Warning("Shader type not found for shader: " + std::string(shaderPath) + " make sure filename follows conventions", "Shader.cpp", __LINE__);
	}
	
	std::stringstream shaderStream;
	// read file's buffer contents into streams
	shaderStream << shaderFile.rdbuf();

	// close file handlers
	shaderFile.close();
	
	// convert stream into string
	const std::string& shaderCode = shaderStream.str();


	const char* sCode = shaderCode.c_str();


	//Making the shader
	shaderID = glCreateShader(shaderType);

	if(shaderID == 0)
	{
		EngineLogger::Error("Didn't create a new shader", "Shader.cpp", __LINE__);
	}

	glShaderSource(shaderID, 1, &sCode, 0);
	glCompileShader(shaderID);
	
	//check for errors
	GLint status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if(status == 0)
	{
		GLsizei errorLogSize = 0;
		GLsizei titleLength;
		std::string errorLog = shaderPath;
		errorLog.append(": ");
		titleLength = errorLog.length();
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
		errorLog.resize( titleLength + errorLogSize );
		glGetShaderInfoLog(shaderID, errorLogSize, &errorLogSize, &errorLog[titleLength]);
		EngineLogger::Error(errorLog, "Shader.cpp", __LINE__);
	}
	
	return shaderID;
}

