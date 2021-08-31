#include "Box.h"
#include "components/Components.h"

using namespace MATH;

Box::Box()
{
}

Box::Box(std::string name, MATH::Vec3 position)
{
	AddComponent<MeshRenderer>()->LoadModel("Cube.fbx");
	GetComponent<MeshRenderer>()->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");

	AddComponent<RigidBody3D>();
	this->name = name;
	transform.SetPos(position);

	transform.scale = Vec3(1.0f, 1.0f, 1.0f);

	canBeInstantiated = true;
}

Box::~Box()
{
}

void Box::OnCollisionEnter(RigidBody3D& otherBody)
{
	//std::cout << this->name << " Collided With: " << otherBody.gameobject->name << std::endl;
}
