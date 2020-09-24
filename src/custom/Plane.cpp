#include "Plane.h"

Plane::Plane()
{
}

Plane::Plane(const char* name, MATH::Vec3 position) : MeshRenderer("resources/models/Plane.fbx")
{
	this->name = name;
	transform.setPos(position);
	transform.scale.print();
	transform.rotation = Vec3(90.0f, 0.0f, 0.0f);
	collider.colliderShape = Collider3D::shape::Plane;

	RigidBody3D::Init(this);
	RigidBody3D::isMoveable() == false;
	MeshRenderer::Init(this);
	MeshRenderer::CreateShader("src/graphics/shaders/BaseVert.glsl", "src/graphics/shaders/BaseFrag.glsl");
	

	
	// Sets the collider up properly when scaled or rotated
	
}

Plane::~Plane()
{
}

void Plane::Update(const float deltaTime)
{
	//transform.rotation.y += deltaTime;
	transform.Update(deltaTime);
	MeshRenderer::Update(deltaTime);
	RigidBody3D::Update(deltaTime);
}

void Plane::Render() const
{
	MeshRenderer::Render();
}

void Plane::HandleEvents(const SDL_Event& event)
{
	MeshRenderer::HandleEvents(event);
}

void Plane::OnCollisionEnter(RigidBody3D& otherBody)
{
	
	//std::cout << "Hit " << otherBody.gameobject->name << std::endl;
}
