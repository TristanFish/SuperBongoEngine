#include "LightObject.h"

using namespace MATH;


LightObject::LightObject(const std::string& name, Vec3 position)
{
	this->name = name;
	transform.SetPos(position);
	AddComponent<LightComponent>();
	
	MeshRenderer* mr = AddComponent<MeshRenderer>();
	mr->LoadModel("Cube.obj");
	mr->SetColorTint(Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	mr->renderFlags = RP_NONE;
	transform.GetPosition() = position;
}

void LightObject::DrawDebugGeometry() const
{

}