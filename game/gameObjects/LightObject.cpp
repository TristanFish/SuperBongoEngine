#include "LightObject.h"

using namespace MATH;


LightObject::LightObject(std::string name, Vec3 position)
{
	AddComponent<LightComponent>();
	this->name = name;
	transform.pos = position;
}

void LightObject::DrawDebugGeometry() const
{
	light.DrawCube(transform.pos, Vec3(1.0f), false, Vec4(1.0f, 1.0f, 0.0f, 1.0f));
}