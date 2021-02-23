#include "Debug.h"
#include <glew/glew.h>
#include "custom/Camera.h"
Shader Debug::TexShader;
Shader Debug::ObjShader;

Debug::~Debug()
{
	if (cubeVAO != 0)
	{
		glDeleteBuffers(1, &cubeVBO);
		glDeleteVertexArrays(1, &cubeVAO);
	}

	if (textureVAO != 0)
	{
		glDeleteBuffers(1, &textureVBO);
		glDeleteVertexArrays(1, &textureVAO);
	}
}

void Debug::DrawSphere(MATH::Vec3 position, float radius, bool wireFrame, Vec4 color) const
{

}

//Function that draws a texture onto the screen, give positions as values between -1 and 1
//at the moment only works for one texture at a time
void Debug::DrawTextureToScreen(GLuint texID, float left, float right, float bot, float top) const
{
	
	if (TexShader.GetID() == 0)
	{
		TexShader.CreateShader("src/graphics/shaders/2DTextureVert.glsl", "src/graphics/shaders/2DTextureFrag.glsl");
	}
	if (textureVAO == 0)
	{
		float quadVertices[] =
		{	//positions							//uvs
			left,  top, -0.9f,		0.0f, 0.0f,
			left,  bot, -0.9f,		0.0f, 1.0f,
			right, top, -0.9f,		1.0f, 0.0f,
			right, bot, -0.9f,		1.0f, 1.0f,
		};

		glGenVertexArrays(1, &textureVAO);
		glGenBuffers(1, &textureVBO);

		glBindVertexArray(textureVAO);
		glBindBuffer(GL_ARRAY_BUFFER, textureVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		glBindVertexArray(0);
	}
	TexShader.RunShader();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	glBindVertexArray(textureVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);
}

void Debug::DrawCube(Vec3 position, Vec3 size, bool wireFrame, Vec4 color) const
{
	if (ObjShader.GetID() == 0)
	{
		ObjShader.CreateShader("src/graphics/shaders/DefaultVert.glsl", "src/graphics/shaders/DefaultFrag.glsl");
	}
	if (cubeVAO == 0)
	{
		float cubeVertices[] =
		{	//positions										
			-1.0f, -1.0f, -1.0f,  // bottom-left
			 1.0f,  1.0f, -1.0f,  // top-right
			 1.0f, -1.0f, -1.0f,  // bottom-right         
			 1.0f,  1.0f, -1.0f,  // top-right
			-1.0f, -1.0f, -1.0f,  // bottom-left
			-1.0f,  1.0f, -1.0f,  // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  // bottom-left
			 1.0f, -1.0f,  1.0f,  // bottom-right
			 1.0f,  1.0f,  1.0f,  // top-right
			 1.0f,  1.0f,  1.0f,  // top-right
			-1.0f,  1.0f,  1.0f,  // top-left
			-1.0f, -1.0f,  1.0f,  // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f,  // top-right
			-1.0f,  1.0f, -1.0f,  // top-left
			-1.0f, -1.0f, -1.0f,  // bottom-left
			-1.0f, -1.0f, -1.0f,  // bottom-left
			-1.0f, -1.0f,  1.0f,  // bottom-right
			-1.0f,  1.0f,  1.0f,  // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  // top-left
			 1.0f, -1.0f, -1.0f,  // bottom-right
			 1.0f,  1.0f, -1.0f,  // top-right         
			 1.0f, -1.0f, -1.0f,  // bottom-right
			 1.0f,  1.0f,  1.0f,  // top-left
			 1.0f, -1.0f,  1.0f,  // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  // top-right
			 1.0f, -1.0f, -1.0f,  // top-left
			 1.0f, -1.0f,  1.0f,  // bottom-left
			 1.0f, -1.0f,  1.0f,  // bottom-left
			-1.0f, -1.0f,  1.0f,  // bottom-right
			-1.0f, -1.0f, -1.0f,  // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  // top-left
			 1.0f,  1.0f , 1.0f,  // bottom-right
			 1.0f,  1.0f, -1.0f,  // top-right     
			 1.0f,  1.0f,  1.0f,  // bottom-right
			-1.0f,  1.0f, -1.0f,  // top-left
			-1.0f,  1.0f,  1.0f  // bottom-left        
		};

		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);

		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); 
		
		glBindVertexArray(0);
	}

	Matrix4 model;
	model.loadIdentity();
	model = model * MMath::translate(position) * MMath::scale(size);

	ObjShader.RunShader();
	ObjShader.TakeUniform("modelMatrix", model);
	ObjShader.TakeUniform("viewMatrix", Camera::getInstance()->getViewMatrix());
	ObjShader.TakeUniform("projectionMatrix", Camera::getInstance()->getProjectionMatrix());
	ObjShader.TakeUniform("meshColor", color);
	
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(cubeVAO);
	if (wireFrame)
	{
		glDrawArrays(GL_LINES, 0, 12 * 3);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	}
	glBindVertexArray(0);

	glUseProgram(0);

}

void Debug::DrawLine(Vec3 start, Vec3 end, Vec4 color)
{
}
