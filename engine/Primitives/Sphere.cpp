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


	this->name = name;
	transform.SetPos(position);

	transform.SetRot(Vec3(1.0f, 1.0f, 1.0f));

	canBeInstantiated = true;
}

Sphere::~Sphere()
{
}

void Sphere::OnCollisionEnter(Collider3D& otherBody)
{
	//std::cout << name << " Collided With: " << otherBody.gameObject->name << std::endl;
}
