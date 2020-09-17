#include "TestModel.h"

TestModel::TestModel()
{

}

TestModel::TestModel(const char* name, MATH::Vec3 position) : MeshRenderer("resources/models/PuffinBall.fbx")
{
	this->name = name;
	transform.setPos(position);

	MeshRenderer::Init(this);
	MeshRenderer::CreateShader("src/graphics/shaders/DefaultVert.glsl", "src/graphics/shaders/DefaultFrag.glsl");
}

TestModel::~TestModel()
{

}

void TestModel::Update(const float deltaTime)
{
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
