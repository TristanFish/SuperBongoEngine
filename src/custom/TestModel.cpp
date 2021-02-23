#include "TestModel.h"

TestModel::TestModel()
{

}

TestModel::TestModel(const char* name, MATH::Vec3 position) : MeshRenderer("Cube.fbx")
{
	this->name = name;
	transform.setPos(position);
	//RigidBody3D::setColliderShape(Collider3D::shape::Sphere);

	RigidBody3D::Init(this);
	//RigidBody3D::ApplyConstantForce(Vec3(0.0, -1.0, 0.0));
	MeshRenderer::renderFlags = RenderProperties::OVERRIDE_RENDERER;
	MeshRenderer::Init(this);
	MeshRenderer::CreateShader("src/graphics/shaders/FogVert.glsl", "src/graphics/shaders/FogFrag.glsl");
}

TestModel::~TestModel()
{
}

void TestModel::AttachUniforms() const
{
	
	shader.TakeUniform("Fog.maxDist", 50.0f);
	shader.TakeUniform("Fog.minDist", 10.0f);
	shader.TakeUniform("Fog.color", Vec3(1.0f, 1.0f, 0.0f));
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




