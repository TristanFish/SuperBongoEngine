#include "Sphere.h"
#include "components/Components.h"

using namespace MATH;

Sphere::Sphere()
{
}

Sphere::Sphere(const char* name, MATH::Vec3 position)
{
	AddComponent<MeshRenderer>()->LoadModel("Sphere.fbx");
	GetComponent<MeshRenderer>()->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");
	this->name = name;
	transform.SetPos(position);

	transform.scale = Vec3(1.0f, 1.0f, 1.0f);
}

Sphere::~Sphere()
{
}

void Sphere::OnCollisionEnter(RigidBody3D& otherBody)
{
	std::cout << name << " Collided With: " << otherBody.gameobject->name << std::endl;
}
