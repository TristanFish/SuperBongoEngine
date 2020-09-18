#pragma once
#include <glew/glew.h>
#include "math/MMath.h"
#include "graphics/Shader.h"
class Debug
{
public:
	static Shader shader;

	static void DrawSphere(MATH::Vec3 position, float radius, bool wireFrame);
	static void DrawTextureToScreen(GLuint texID, MATH::Vec2 topLeft, MATH::Vec2 topRight, MATH::Vec2 botRight, MATH::Vec2 botLeft);
	static void DrawCube(MATH::Vec3 position, MATH::Vec3 size, bool wireFrame);
};

