#include "Sphere.h"
#include "components/Components.h"

using namespace MATH;

Sphere::Sphere()
{
}

Sphere::Sphere(const std::string& name, MATH::Vec3 position)
{
	AddComponent<MeshRenderer>()->LoadModel("Sphere.fbx");
	GetComponent<MeshRenderer>()->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");
	//AddComponent<RigidBody3D>();
	//GetComponent<RigidBody3D>()->ApplyConstantTorque(Vec3(0.0f, 20.0, 10.0));

	this->name = name;
	transform.SetPos(position);

	transform.scale = Vec3(1.0f, 1.0f, 1.0f);

	canBeInstantiated = true;
}

Sphere::~Sphere()
{
}

void Sphere::OnCollisionEnter(RigidBody3D& otherBody)
{
	std::cout << name << " Collided With: " << otherBody.gameObject->name << std::endl;
}
