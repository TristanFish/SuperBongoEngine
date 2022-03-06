#include "Box.h"
#include "components/Components.h"

using namespace MATH;

Box::Box()
{
}

Box::Box(const std::string& name, Vec3 position)
{
	AddComponent<MeshRenderer>()->LoadModel("Cube.obj");
	GetComponent<MeshRenderer>()->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");
	AddComponent<RigidBody3D>();
	this->name = name;
	transform.SetPos(position);

	canBeInstantiated = true;
}

Box::~Box()
{
}

void Box::PostInit()
{
	GetComponent<RigidBody3D>()->ConstructCollider(ColliderType::OBB);

	GameObject::PostInit();
}

void Box::OnCollisionEnter(Collider3D& otherBody)
{
	//EngineLogger::Info(this->name + " Collided With " + otherBody.GetRBAttached()->gameObject->name  , "Box.cpp", __LINE__, MessageTag::TYPE_NONE);

}
