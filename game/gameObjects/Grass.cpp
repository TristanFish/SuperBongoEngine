#include "Grass.h"

using namespace MATH;


Grass::Grass(std::string name, MATH::Vec3 position, unsigned int amount_)
{
	AddComponent<MeshRenderer>()->LoadModel("GrassBunch1.fbx");
	GetComponent<MeshRenderer>()->CreateShader("GrassVert.glsl", "GrassFrag.glsl");
	GetComponent<MeshRenderer>()->renderFlags = RenderProperties::RP_OVERRIDE_RENDERER;
	GetComponent<MeshRenderer>()->SetInstanceID(1);
	GetComponent<MeshRenderer>()->SetInstanceAmount(amount_);
	amount = amount_;
	
	this->name = name;
	transform.SetPos(position);
	transform.SetRot(Vec3(-90.0f, 0.0f, 0.0f));
	transform.scale = Vec3(5.0f);

	canBeInstantiated = true;

	
	Instancer::Init(amount_, this);
}