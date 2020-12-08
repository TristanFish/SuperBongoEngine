#include "Water.h"

Water::Water(const char* name, MATH::Vec3 pos): MeshRenderer("Plane.fbx") {
	this->name = name;
	this->transform.pos = pos;
	transform.setRot(Vec3(-90.0f, 0.0f, 0.0f));

	MeshRenderer::renderFlags = RenderProperties::WATER;
	MeshRenderer::Init(this);
	MeshRenderer::CreateShader("src/graphics/shaders/WaterVert.glsl", "src/graphics/shaders/WaterFrag.glsl");
	MeshRenderer::SetInstanceID(8);

}

Water::~Water()
{

}

void Water::Update(const float deltaTime)
{
}

void Water::Render() const
{
	MeshRenderer::Render();
}

void Water::HandleEvents(const SDL_Event& event)
{
}
