#include "Plane.h"

Plane::Plane()
{
}

Plane::Plane(const char* name, MATH::Vec3 position) : MeshRenderer("resources/models/Plane.fbx")
{
	this->name = name;
	transform.setPos(position);
	transform.scale =  Vec3(1.0f, 1.0f, 1.0f);
	transform.rotation = Vec3(90.0f, 0.0f, 0.0f);
	transform.rotation.print();
	RigidBody3D::Init(this);
	RigidBody3D::setMoveable(false);
	MeshRenderer::Init(this);
	MeshRenderer::CreateShader("src/graphics/shaders/DefaultVert.glsl", "src/graphics/shaders/DefaultFrag.glsl");
	MeshRenderer::p_max = ((MMath::calcRotationMatrix(transform.rotation) * MeshRenderer::p_max) * *transform.scale);
	MeshRenderer::p_min = ((MMath::calcRotationMatrix(transform.rotation) * MeshRenderer::p_min) * *transform.scale);
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
