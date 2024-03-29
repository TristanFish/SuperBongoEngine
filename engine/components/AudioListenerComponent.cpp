#include "AudioListenerComponent.h"
#include "GameObject.h"



void AudioListenerComponent::Init(std::shared_ptr<GameObject> g)
{
	gameObject = g;
	audioSystemRef = AudioManager::Get()->system;

	//could just as easily be the camera
	listenerPos = &g->transform.GetPositionRef();
	//FMODForward = (FMOD_VECTOR*)&g->transform.Forward();
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
	FMODlistenerPos = reinterpret_cast<FMOD_VECTOR*>(listenerPos);
	//Should probably pass in the camera members but this is fine for now.
	audioSystemRef->set3DListenerAttributes(0, FMODlistenerPos, 0, &forwardVec, &FMODUp);
	
}