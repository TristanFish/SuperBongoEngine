#include "Debug.h"
#include <glew/glew.h>

Shader Debug::shader;

void Debug::DrawSphere(MATH::Vec3 position, float radius, bool wireFrame)
{
}


unsigned int quadVAO = 0;
unsigned int quadVBO;
//Function that draws a texture onto the screen, give positions as values between -1 and 1
//at the moment only works for one texture at a time
void Debug::DrawTextureToScreen(GLuint texID, MATH::Vec2 topLeft, MATH::Vec2 topRight, MATH::Vec2 botRight, MATH::Vec2 botLeft)
{
	if (shader.GetID() == 0)
	{
		shader.CreateShader("src/graphics/shaders/2DTextureVert.glsl", "src/graphics/shaders/2DTextureFrag.glsl");
	}
	if (quadVAO == 0)
	{
		float quadVertices[] =
		{	//positions							//uvs
			topLeft.x, topLeft.y, 0.0f,			0.0f, 0.0f,
			botLeft.x, botLeft.y, 0.0f,			0.0f, 1.0f,
			topRight.x, topRight.y, 0.0f,		1.0f, 0.0f,
			botRight.x, botRight.y, 0.0f,		1.0f, 1.0f,
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	shader.RunShader();
	glBindTexture(GL_TEXTURE_2D, texID);

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Debug::DrawCube(MATH::Vec3 position, MATH::Vec3 size, bool wireFrame)
{
}
