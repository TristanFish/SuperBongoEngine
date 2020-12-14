#include "Bird.h"
Bird::Bird(const char* name, MATH::Vec3 position): MeshRenderer("Bird.obj") {
	this->name = name;
	this->transform.pos = position;
	MeshRenderer::Init(this);
	MeshRenderer::CreateShader("src/graphics/shaders/DefaultVert.glsl", "src/graphics/shaders/DefaultFrag.glsl");
	AudioSourceComponent::Init(this);
	birdSound = AudioManager::Get()->RetrieveSoundObject("birdchirp");
	AudioSourceComponent::PlaySound(birdSound, &AudioManager::Get()->C1);
	this->transform.setRot(MATH::Vec3(-90.0f, 0.0f, 90.0f));
	this->transform.scale = MATH::Vec3(0.1f, 0.1f, 0.1f);
	this->transform.setPos(MATH::Vec3(0.0f, 6.0f, 0.0f));

}

Bird::~Bird()
{
}

void Bird::Update(const float deltaTime)
{
	//bool isDonePlaying = false;
	//isDonePlaying = AudioManager::Get()->MonitorChannel(*AudioManager::Get()->C1);
	//if (isDonePlaying) { AudioSourceComponent::PlaySound(birdSound, &AudioManager::Get()->C1); }
	transform.Update(deltaTime);
	MeshRenderer::Update(deltaTime);
}

void Bird::Render() const
{

}

void Bird::HandleEvents(const SDL_Event& event)
{
}
