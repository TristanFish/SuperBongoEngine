#include "TestModel.h"

TestModel::TestModel()
{

}

TestModel::TestModel(const char* name, MATH::Vec3 position) : MeshRenderer("Cube.fbx")
{
	this->name = name;
	transform.setPos(position);
	//RigidBody3D::setColliderShape(Collider3D::shape::Sphere);


	MeshRenderer::renderFlags = RenderProperties::OVERRIDE_RENDERER;
	MeshRenderer::Init(this);
	MeshRenderer::CreateShader("src/graphics/shaders/FogVert.glsl", "src/graphics/shaders/FogFrag.glsl");
}

TestModel::~TestModel()
{
}

void TestModel::AttachUniforms() const
{
	
	shader.TakeInUniformFloat("Fog.maxDist", 50.0f);
	shader.TakeInUniformFloat("Fog.minDist", 10.0f);
	shader.TakeInUniformVec3("Fog.color", Vec3(1.0f, 1.0f, 0.0f));
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




