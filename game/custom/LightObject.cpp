#include "LightObject.h"

using namespace MATH;


LightObject::LightObject(const char* name, Vec3 position)
{
	AddComponent<LightComponent>();
	this->name = name;
	transform.pos = position;
}

void LightObject::DrawDebugGeometry() const
{
	light.DrawCube(transform.pos, Vec3(2.0f), false, Vec4(1.0f, 1.0f, 0.0f, 1.0f));
}