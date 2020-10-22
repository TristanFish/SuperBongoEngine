#include "Water.h"

Water::Water(const char* name, MATH::Vec3 pos) {
	this->name = name;
	this->transform.pos = pos;
	transform.setRot(Vec3(-90.0f, 0.0f, 0.0f));

	MeshRenderer::renderFlags = RenderProperties::WATER;
	MeshRenderer::Init(this);
	MeshRenderer::CreateShader("src/graphics/shaders/GrassVert.glsl", "src/graphics/shaders/GrassFrag.glsl");
	MeshRenderer::SetInstanceID(2);

}

Water::~Water()
{
	delete(this);
}

void Water::Update(const float deltaTime)
{
}

void Water::Render() const
{
}

void Water::HandleEvents(const SDL_Event& event)
{
}
