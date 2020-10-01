#include "AudioListenerComponent.h"

void AudioListenerComponent::Init(GameObject* g)
{
	gameobject = g;
	audioSystemRef = AudioManager::Get()->system;

	//could just as easily be the camera
	listenerPos = &g->transform.pos;
	FMODForward = (FMOD_VECTOR*)&g->transform.rotation;
	FMODUp = {0.0f, 1.0f , 0.0f};

}

void AudioListenerComponent::Update(const float deltaTime)
{
	SetListenerPosition();
}


void AudioListenerComponent::SetListenerPosition()
{
	FMODlistenerPos = (FMOD_VECTOR*)listenerPos;
	//Should probably pass in the camera members but this is fine for now.
	audioSystemRef->set3DListenerAttributes(0, FMODlistenerPos, 0, FMODForward, &FMODUp);
}

//Nothing to render
void AudioListenerComponent::Render(){}

//Nothing to handle for now, maybe an audio event system later
void AudioListenerComponent::HandleEvents(const SDL_Event& event){}