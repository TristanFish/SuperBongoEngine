#include "AudioListenerComponent.h"
#include "GameObject.h"

void AudioListenerComponent::Init(GameObject* g)
{
	gameObject = g;
	audioSystemRef = AudioManager::Get()->system;

	//could just as easily be the camera
	listenerPos = &g->transform.GetPosition();
	FMODForward = (FMOD_VECTOR*)&g->transform.GetRotationQuat();
	FMODUp = {0.0f, 1.0f , 0.0f};

}

void AudioListenerComponent::Update(const float deltaTime)
{
	SetListenerPosition();
	AudioManager::Get()->Update();
}


void AudioListenerComponent::SetListenerPosition()
{
	FMOD_VECTOR forwardVec{0.0f, 0.0f ,1.0f};
	FMODlistenerPos = (FMOD_VECTOR*)listenerPos;
	//Should probably pass in the camera members but this is fine for now.
	audioSystemRef->set3DListenerAttributes(0, FMODlistenerPos, 0, &forwardVec, &FMODUp);
	
}