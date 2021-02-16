#include "Sphere.h"

Sphere::Sphere()
{
}

Sphere::Sphere(const char* name, MATH::Vec3 position) : MeshRenderer("Sphere.fbx")
{
	this->name = name;
	transform.setPos(position);

	transform.scale = Vec3(1.0f, 1.0f, 1.0f);

	RigidBody3D::Init(this);
	MeshRenderer::Init(this);

	MeshRenderer::CreateShader("src/graphics/shaders/DefaultVert.glsl", "src/graphics/shaders/DefaultFrag.glsl");

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
