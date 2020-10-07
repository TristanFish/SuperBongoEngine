#include "TestModel.h"

TestModel::TestModel()
{

}

TestModel::TestModel(const char* name, MATH::Vec3 position) : MeshRenderer("resources/models/Latch.obj")
{
	this->name = name;
	transform.setPos(position);
	transform.rotation = Vec3(0.0f, 0.0f, 0.0f);
	transform.scale =  Vec3(0.5f, 0.5f, 0.5f);
	RigidBody3D::Init(this);
	RigidBody3D::SetAccel(MATH::Vec3(0.0f, -9.8f, 0.0f));
	//RigidBody3D::setColliderShape(Collider3D::shape::Sphere);
	MeshRenderer::Init(this);
	MeshRenderer::CreateShader("src/graphics/shaders/DefaultVert.glsl", "src/graphics/shaders/DefaultFrag.glsl");
	MeshRenderer::p_max = ((MMath::calcRotationMatrix(transform.rotation) * MeshRenderer::p_max) * *transform.scale);
	MeshRenderer::p_min = ((MMath::calcRotationMatrix(transform.rotation) * MeshRenderer::p_min) * *transform.scale);
}

TestModel::~TestModel()
{
}

void TestModel::Update(const float deltaTime)
{
	//transform.rotation.y += deltaTime;
	transform.Update(deltaTime);
	MeshRenderer::Update(deltaTime);
	RigidBody3D::Update(deltaTime);
}

void TestModel::Render() const
{
	MeshRenderer::Render();
}

void TestModel::HandleEvents(const SDL_Event& event)
{
	MeshRenderer::HandleEvents(event);
}

void TestModel::OnCollisionEnter(RigidBody3D& otherBody)
{
	

}
