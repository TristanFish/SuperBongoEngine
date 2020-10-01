#include "TestModel.h"

TestModel::TestModel()
{

}

TestModel::TestModel(const char* name, MATH::Vec3 position) : MeshRenderer("resources/models/Cube.fbx")
{
	this->name = name;
	transform.setPos(position);
	//RigidBody3D::setColliderShape(Collider3D::shape::Sphere);
	MeshRenderer::Init(this);
	MeshRenderer::CreateShader("src/graphics/shaders/FogVert.glsl", "src/graphics/shaders/FogFrag.glsl");

	// Sets the collider up properly when scaled or rotated
	
}

TestModel::~TestModel()
{
}

void TestModel::Update(const float deltaTime)
{
	//transform.rotation.y += deltaTime;
	transform.Update(deltaTime);
	MeshRenderer::Update(deltaTime);
}

void TestModel::Render() const
{
	MeshRenderer::Render();
}

void TestModel::HandleEvents(const SDL_Event& event)
{
	MeshRenderer::HandleEvents(event);
}


