#include "Tetrahedron.h"

#include "components/Components.h"

using namespace MATH;

Tetrahedron::Tetrahedron()
{
}

Tetrahedron::Tetrahedron(const std::string& name, MATH::Vec3 position)
{
	AddComponent<MeshRenderer>()->LoadModel("Tetrahedron.fbx");
	GetComponent<MeshRenderer>()->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");
	AddComponent<RigidBody3D>();
	GetComponent<RigidBody3D>()->ApplyConstantTorque(Vec3(0.0f, 20.0, 0.0));

	this->name = name;
	transform.SetPos(position);

	transform.SetScale(Vec3(25.0f, 25.0f, 25.0f));

	canBeInstantiated = true;
}

Tetrahedron::~Tetrahedron()
{
}

void Tetrahedron::OnCollisionEnter(Collider3D& otherBody)
{
	//std::cout << name << " Collided With: " << otherBody.gameObject->name << std::endl;
}

void Tetrahedron::PostInit()
{
	GetComponent<RigidBody3D>()->ConstructCollider(ColliderType::OBB);

	GameObject::PostInit();
}
