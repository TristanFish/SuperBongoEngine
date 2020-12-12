#include "Bird.h"
Bird::Bird(const char* name, MATH::Vec3 position): MeshRenderer("Bird.obj") {
	this->name = name;
	this->transform.pos = position;
	MeshRenderer::Init(this);
	MeshRenderer::CreateShader("src/graphics/shaders/DefaultVert.glsl", "src/graphics/shaders/DefaultFrag.glsl");
	AudioSourceComponent::Init(this);
	//AudioSourceComponent::PlaySound("birdchirp");
}

Bird::~Bird()
{
}

void Bird::Update(const float deltaTime)
{
}

void Bird::Render() const
{
}

void Bird::HandleEvents(const SDL_Event& event)
{
}
