#include "TestModel.h"

TestModel::TestModel()
{

}

TestModel::TestModel(const char* name, MATH::Vec3 position) : MeshRenderer("resources/models/PuffinBall.fbx")
{
	this->name = name;
	transform.setPos(position);
	RigidBody3D::Init(this);
	RigidBody3D::SetAccel(MATH::Vec3(0.0f, 0.0f, -1.0f));
	MeshRenderer::Init(this);
	MeshRenderer::CreateShader("src/graphics/shaders/DefaultVert.glsl", "src/graphics/shaders/DefaultFrag.glsl");
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
