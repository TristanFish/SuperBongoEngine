#pragma once
#include <glew/glew.h>
#include "math/MMath.h"
#include "graphics/Shader.h"
class Debug
{
public:
	static Shader shader;

	static void DrawSphere(Vec3 position, float radius, bool wireFrame, Vec3 color = Vec3(1.0f, 1.0f, 1.0f));
	//Function that draws a texture onto the screen, give positions as values between -1 and 1
	//at the moment only works for one texture at a time
	static void DrawTextureToScreen(GLuint texID, Vec2 topLeft, Vec2 topRight, Vec2 botRight, Vec2 botLeft);
	static void DrawCube(Vec3 position, Vec3 size, bool wireFrame, Vec3 color = Vec3(1.0f, 1.0f, 1.0f));
};

