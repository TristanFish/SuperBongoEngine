#include "Debug.h"
#include <glew/glew.h>
#include "custom/Camera.h"
Shader Debug::TexShader;
Shader Debug::ObjShader;

void Debug::DrawSphere(MATH::Vec3 position, float radius, bool wireFrame, Vec3 color)
{

}


GLuint textureVAO = 0;
GLuint textureVBO;
//Function that draws a texture onto the screen, give positions as values between -1 and 1
//at the moment only works for one texture at a time
void Debug::DrawTextureToScreen(GLuint texID, MATH::Vec2 topLeft, MATH::Vec2 topRight, MATH::Vec2 botRight, MATH::Vec2 botLeft)
{
	if (TexShader.GetID() == 0)
	{
		TexShader.CreateShader("src/graphics/shaders/2DTextureVert.glsl", "src/graphics/shaders/2DTextureFrag.glsl");
	}
	if (textureVAO == 0)
	{
		float quadVertices[] =
		{	//positions							//uvs
			topLeft.x, topLeft.y, 0.0f,			0.0f, 0.0f,
			botLeft.x, botLeft.y, 0.0f,			0.0f, 1.0f,
			topRight.x, topRight.y, 0.0f,		1.0f, 0.0f,
			botRight.x, botRight.y, 0.0f,		1.0f, 1.0f,
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
	glBindTexture(GL_TEXTURE_2D, texID);

	glBindVertexArray(textureVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint cubeVAO = 0;
GLuint cubeVBO;
void Debug::DrawCube(Vec3 position, Vec3 size, bool wireFrame, Vec4 color)
{
	if (TexShader.GetID() == 0)
	{
		TexShader.CreateShader("src/graphics/shaders/DefaultVert.glsl", "src/graphics/shaders/DefaultFrag.glsl");
	}
	if (textureVAO == 0)
	{
		float cubeVertices[] =
		{	//positions										
			(-size.x / 2.0f),	(-size.y / 2.0f),	(-size.z / 2.0f),
			(-size.x / 2.0f),	(-size.y / 2.0f),	( size.z / 2.0f),
			(-size.x / 2.0f),	( size.y / 2.0f),	( size.z / 2.0f),
			(size.x / 2.0f),	( size.y / 2.0f),	(-size.z / 2.0f), 
			(size.x / 2.0f),	(-size.y / 2.0f),	(-size.z / 2.0f),
			(size.x / 2.0f),	( size.y / 2.0f),	(-size.z / 2.0f),
			(size.x / 2.0f),	(-size.y / 2.0f),	( size.z / 2.0f),
			(-size.x / 2.0f),	(-size.y / 2.0f),	(-size.z / 2.0f),
			(size.x / 2.0f),	(-size.y / 2.0f),	(-size.z / 2.0f),
			(size.x / 2.0f),	( size.y / 2.0f),	(-size.z / 2.0f),
			(size.x / 2.0f),	(-size.y / 2.0f),	(-size.z / 2.0f),
			(-size.x / 2.0f),	(-size.y / 2.0f),	(-size.z / 2.0f),
			(-size.x / 2.0f),	(-size.y / 2.0f),	(-size.z / 2.0f),
			(-size.x / 2.0f),	( size.y / 2.0f),	( size.z / 2.0f),
			(-size.x / 2.0f),	( size.y / 2.0f),	(-size.z / 2.0f),
			(size.x / 2.0f),	(-size.y / 2.0f),	( size.z / 2.0f),
			(-size.x / 2.0f),	(-size.y / 2.0f),	( size.z / 2.0f),
			(-size.x / 2.0f),	(-size.y / 2.0f),	(-size.z / 2.0f),
			(-size.x / 2.0f),	( size.y / 2.0f),	( size.z / 2.0f),
			(-size.x / 2.0f),	(-size.y / 2.0f),	( size.z / 2.0f),
			(size.x / 2.0f),	(-size.y / 2.0f),	( size.z / 2.0f),
			(size.x / 2.0f),	( size.y / 2.0f),	( size.z / 2.0f),
			(size.x / 2.0f),	(-size.y / 2.0f),	(-size.z / 2.0f),
			(size.x / 2.0f),	( size.y / 2.0f),	(-size.z / 2.0f),
			(size.x / 2.0f),	(-size.y / 2.0f),	(-size.z / 2.0f),
			(size.x / 2.0f),	( size.y / 2.0f),	( size.z / 2.0f),
			(size.x / 2.0f),	(-size.y / 2.0f),	( size.z / 2.0f),
			(size.x / 2.0f),	( size.y / 2.0f),	( size.z / 2.0f),
			(size.x / 2.0f),	( size.y / 2.0f),	(-size.z / 2.0f),
			(-size.x / 2.0f),	( size.y / 2.0f),	(-size.z / 2.0f),
			(size.x / 2.0f),	( size.y / 2.0f),	( size.z / 2.0f),
			(-size.x / 2.0f),	( size.y / 2.0f),	(-size.z / 2.0f),
			(-size.x / 2.0f),	( size.y / 2.0f),	( size.z / 2.0f),
			(size.x / 2.0f),	( size.y / 2.0f),	( size.z / 2.0f),
			(-size.x / 2.0f),	( size.y / 2.0f),	( size.z / 2.0f),
			(size.x / 2.0f),	(-size.y / 2.0f),	( size.z / 2.0f)
		};

		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);

		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindVertexArray(0);
	}

	Matrix4 model;
	model.loadIdentity();
	model = model * MMath::translate(position);

	TexShader.RunShader();
	TexShader.TakeInUniformMat4("modelMatrix", model);
	TexShader.TakeInUniformMat4("viewMatrix", Camera::getInstance()->getViewMatrix());
	TexShader.TakeInUniformMat4("viewMatrix", Camera::getInstance()->getProjectionMatrix());
	//TexShader.TakeInUniformVec4("meshColor", color);
	
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(cubeVAO);
	if (!wireFrame)
	{
		glDrawArrays(GL_LINES, 0, 12 * 3);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	}
	glBindVertexArray(0);

}
