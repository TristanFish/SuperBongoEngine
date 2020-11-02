#pragma once
#include <glew/glew.h>
#include "math/MMath.h"
#include "graphics/Shader.h"
class Debug
{
public:
	static Shader TexShader;
	static Shader ObjShader;

	mutable GLuint textureVAO = 0;
	mutable GLuint textureVBO;

	mutable GLuint cubeVAO = 0;
	mutable GLuint cubeVBO;
	
	~Debug();

	void DrawSphere(Vec3 position, float radius, bool wireFrame, Vec4 color = Vec4(1.0f, 1.0f, 1.0f, 1.0f)) const;
	//Draws a texture onto the screen, give positions as values between -1 and 1
	//at the moment only works for one texture at a time
	void DrawTextureToScreen(GLuint texID, float left = -1.0f, float right = -0.5f, float bot = -1.0f, float top = -0.5f) const;

	//Draws a cube to the screen
	void DrawCube(Vec3 position, Vec3 size, bool wireFrame, Vec4 color = Vec4(1.0f, 1.0f, 1.0f, 1.0f)) const;

	void DrawLine(Vec3 start, Vec3 end, Vec4 color = Vec4(1.0f, 0.0f, 0.0f, 1.0f));

};

