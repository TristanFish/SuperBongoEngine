#include "AudioSourceComponent.h"

void AudioSourceComponent::Init(GameObject* g)
{


	gameobject = g;
	gameObjectPosition = &g->transform.pos;
	audioSystemRef = AudioManager::Get().system;
	soundAtts.C1->setChannelGroup(soundAtts.G1);
	SetAudioPosition();
}

void AudioSourceComponent::Update(const float deltaTime)
{
	SetAudioPosition();
}

void AudioSourceComponent::LoadSounds()
{
	
}

void AudioSourceComponent::SetAudioPosition()
	
{
	FMOD_RESULT r;
	//Gotta convert to FMODVECTOR, its no problem as long as the vector is always a vec3
	r = soundAtts.C1->set3DAttributes((FMOD_VECTOR*)gameObjectPosition, NULL);
	if (r != FMOD_OK) {
		std::cout << r << FMOD_ErrorString(r) << "-------"<< "Audio Component did not set 3D attributes, game object position unavailable"<< std::endl;
	}
}

