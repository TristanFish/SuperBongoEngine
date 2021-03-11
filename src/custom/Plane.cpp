#include "Plane.h"

using namespace MATH;



Plane::Plane(const char* name, Vec3 position)
{
	AddComponent<MeshRenderer>()->LoadModel("Plane.fbx");
	GetComponent<MeshRenderer>()->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");


	
	AddComponent<RigidBody3D>()->collisionEnterCallback = [](RigidBody3D& a) {};
	
	this->name = name;
	transform.SetPos(position);

	transform.scale =  Vec3(1.0f, 1.0f, 1.0f);
	transform.rotation = Vec3(90.0f, 0.0f, 0.0f);
	transform.rotation.print();

	GameObject::Init();
}

void Plane::operator()(RigidBody3D& otherBody)
{
}

void Plane::OnCollisionEnter(RigidBody3D& otherBody)
{
	
}

const char* Plane::GetClassIDName() const
{
	return "Plane";
}
