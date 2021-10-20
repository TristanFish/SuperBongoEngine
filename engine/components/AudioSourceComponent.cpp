#include "AudioSourceComponent.h"
#include "GameObject.h"
#include "core/Logger.h"


void AudioSourceComponent::Init(GameObject* g)
{
	soundAtts.C1 = nullptr;
	soundAtts.G1 = nullptr;
	soundAtts.reverb = nullptr;
	gameObject = nullptr;
	audioSystemRef = nullptr;

	gameObject = g;
	gameObjectPosition = g->transform.pos;

	audioSystemRef = AudioManager::Get()->system;
	FMODGamePos =  AudioUtility::Vec3ToFMODVec(gameObjectPosition);
	AudioManager::Get()->CreateChannelGroup("Channel Group 1", soundAtts.G1);
	
}

void AudioSourceComponent::Update(const float deltaTime)
{
//Set a check here to see if sound is ready to play? Then update position
}


void AudioSourceComponent::PlaySound(FMOD::Sound* sound, FMOD::Channel** channelRef)
{
	//True is passed in so the sound does not play immediatley, we need to set the channels audio position.
	EngineLogger::Info("Sound Played", "AudioSourceComponent.cpp", __LINE__);
	
	auto r = AudioManager::Get()->system->playSound(sound, NULL, true, channelRef);
	SetAudioPosition();
	(*channelRef)->setPaused(false);
}


FMOD::Sound* AudioSourceComponent::LoadSound(std::string soundName)
{
	FMOD::Sound* sound = AudioManager::Get()->RetrieveSoundObject(soundName);
	if (sound == nullptr) {
		EngineLogger::Warning(soundName + " sound not found make sure you're using a valid sound name.", "AudioSourceComponent.cpp", __LINE__);
		return NULL;
	}
	return sound;
}

FMOD::Sound* AudioSourceComponent::GetSound(std::string soundNameFromMap)
{
	auto sound = LoadSound(soundNameFromMap);
	
	if (sound == nullptr) {
		EngineLogger::Warning("invalid sound element, buffer returned null or invalid name", "AudioSourceComponent.cpp", __LINE__);
	}
	return sound;
}


void AudioSourceComponent::SetAudioPosition()
	
{
	FMOD_RESULT r;
	//Gotta convert to FMODVECTOR, its no problem as long as the vector is always a vec3
	r = AudioManager::Get()->C1->set3DAttributes(&FMODGamePos, NULL);
	if (r != FMOD_OK) {
		EngineLogger::Warning("Audio Component did not set 3D attributes, game object position unavailable", "AudioSourceComponent.cpp", __LINE__);
	}
}

AudioSourceComponent::~AudioSourceComponent()
{
	gameObject = nullptr; 
	audioSystemRef = nullptr;
	soundAtts.C1 = nullptr;
	soundAtts.G1 = nullptr;
	soundAtts.reverb = nullptr;
}

