#include "Sphere.h"

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

	typeID = Type::Sphere;
}

Sphere::~Sphere()
{
}

void Sphere::Update(const float deltaTime)
{
	//transform.rotation.y += deltaTime;
	transform.Update(deltaTime);
	MeshRenderer::Update(deltaTime);
	RigidBody3D::Update(deltaTime);
}

void Sphere::Render() const
{
	MeshRenderer::Render();
}

void Sphere::HandleEvents(const SDL_Event& event)
{
	MeshRenderer::HandleEvents(event);
}

void Sphere::OnCollisionEnter(RigidBody3D& otherBody)
{
	std::cout << name << " Collided With: " << otherBody.gameobject->name << std::endl;
}
