#include "Shader.h"



GLuint Shader::CreateShader(const char* vertexPath, const char* fragmentPath)
{
	static bool shaderisnotcreated = true;
	if (shaderisnotcreated)
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		std::cout << vShaderStream.str() << fShaderStream.str() << std::endl;

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();


		//Making the vertex shader
		int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vShaderCode, 0);
		glCompileShader(vertex_shader);

		// Making the fragment shader
		int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fShaderCode, 0);
		glCompileShader(fragment_shader);

		// link dah shadah
		shader_program = glCreateProgram();
		if (shader_program == 0)
		{
			std::cout << "Shader not loaded" << std::endl;
		}
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
		glLinkProgram(shader_program);

		// Delete
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		shaderisnotcreated = false;
	}
	return shader_program;
}

//only use after creation
void Shader::RunShader() const
{
	glUseProgram(shader_program);
}

void Shader::TakeInUniformMat4(const std::string &name, Matrix4 &matrix)
{
	GLint location = glGetUniformLocation(shader_program, name.c_str());
	if (location < 0) { std::cout << "Uniform ID not found for camera to init" << std::endl; }
	glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
}


