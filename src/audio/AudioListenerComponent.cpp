#include "AudioListenerComponent.h"

void AudioListenerComponent::Init(GameObject* g)
{
	gameobject = g;
	audioSystemRef = AudioManager::Get()->system;

	//Setting the listener position to the player, could just as easily be the camera
	listenerPos = &g->transform.pos;
	FMODForward = (FMOD_VECTOR*)&g->transform.rotation;
	FMODUp = {0.0f, 1.0f , 0.0f};

}

void AudioListenerComponent::Update(const float deltaTime)
{
	SetListenerPosition();
}

void AudioListenerComponent::Render(){}

void AudioListenerComponent::HandleEvents(const SDL_Event& event){}

void AudioListenerComponent::SetListenerPosition()
{
	FMODlistenerPos = (FMOD_VECTOR*)listenerPos;
	audioSystemRef->set3DListenerAttributes(0, FMODlistenerPos, 0, FMODForward, &FMODUp);
}
