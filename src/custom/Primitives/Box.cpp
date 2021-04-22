#include "Box.h"

using namespace MATH;

Box::Box()
{
}

Box::Box(const char* name, MATH::Vec3 position)
{
	AddComponent<MeshRenderer>()->LoadModel("Cube.fbx");
	GetComponent<MeshRenderer>()->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");

	AddComponent<RigidBody3D>();
	this->name = name;
	transform.SetPos(position);

	transform.scale = Vec3(1.0f, 1.0f, 1.0f);

	typeID = Type::Box;

}

Box::~Box()
{
}

void Box::Update(const float deltaTime)
{
	//transform.rotation.y += deltaTime;
	transform.Update(deltaTime);
	MeshRenderer::Update(deltaTime);
	RigidBody3D::Update(deltaTime);
}

void Box::Render() const
{
	MeshRenderer::Render();
}

void Box::HandleEvents(const SDL_Event& event)
{
	MeshRenderer::HandleEvents(event);
}

void Box::OnCollisionEnter(RigidBody3D& otherBody)
{
	//std::cout << this->name << " Collided With: " << otherBody.gameobject->name << std::endl;
}
