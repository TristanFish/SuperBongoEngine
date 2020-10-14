#include "Grass.h"

Grass::Grass()
{

}


Grass::Grass(const char* name, MATH::Vec3 position, unsigned int amount_) : MeshRenderer("GrassBunch1.fbx")
{
	this->name = name;
	transform.setPos(position);
	transform.setRot(Vec3(-90.0f, 0.0f, 0.0f));
	transform.scale = Vec3(5.0f);
	MeshRenderer::renderFlags = RenderProperties::OVERRIDE_RENDERER;
	MeshRenderer::Init(this);
	MeshRenderer::CreateShader("src/graphics/shaders/GrassVert.glsl", "src/graphics/shaders/GrassFrag.glsl");
	MeshRenderer::SetInstanceID(1);

	Instancer::Init(amount_, gameobject);
	MeshRenderer::SetInstanceAmount(amount_);
}

Grass::~Grass()
{
}

void Grass::Update(const float deltaTime)
{
	transform.Update(deltaTime);
	MeshRenderer::Update(deltaTime);
}

void Grass::Render() const
{
	MeshRenderer::Render();
}

void Grass::HandleEvents(const SDL_Event& event)
{
	MeshRenderer::HandleEvents(event);
}

