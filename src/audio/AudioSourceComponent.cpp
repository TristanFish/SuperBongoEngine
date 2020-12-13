#include "AudioSourceComponent.h"

void AudioSourceComponent::Init(GameObject* g)
{
	soundAtts.C1 = nullptr;
	soundAtts.G1 = nullptr;
	soundAtts.reverb = nullptr;
	gameobject = nullptr;
	audioSystemRef = nullptr;

	gameobject = g;
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
	std::cout << "sound played" << std::endl;
	
	auto r = AudioManager::Get()->system->playSound(sound, NULL, true, channelRef);
	std::cout << r << std::endl;
	SetAudioPosition();
	(*channelRef)->setPaused(false);
}


FMOD::Sound* AudioSourceComponent::LoadSound(std::string soundName)
{
	FMOD::Sound* sound = AudioManager::Get()->RetrieveSoundObject(soundName);
	if (sound == nullptr) {
		std::cout << "Sound not found make sure you're using a valid sound name." << std::endl;
		return NULL;
	}
	return sound;
}

FMOD::Sound* AudioSourceComponent::GetSound(std::string soundNameFromMap)
{
	auto sound = LoadSound(soundNameFromMap);
	
	if (sound == nullptr) {
	
		std::cout << "invalid sound element, buffer returned null or invalid name" << std::endl;
	}
	return sound;
}


void AudioSourceComponent::SetAudioPosition()
	
{
	FMOD_RESULT r;
	//Gotta convert to FMODVECTOR, its no problem as long as the vector is always a vec3
	r = AudioManager::Get()->C1->set3DAttributes(&FMODGamePos, NULL);
	if (r != FMOD_OK) {
		std::cout << r << FMOD_ErrorString(r) << "-------"<< "Audio Component did not set 3D attributes, game object position unavailable"<< std::endl;
	}
}


void AudioSourceComponent::Render(){}
void AudioSourceComponent::HandleEvents(const SDL_Event& event){}

AudioSourceComponent::~AudioSourceComponent()
{
	gameobject = nullptr; 
	audioSystemRef = nullptr;
	soundAtts.C1 = nullptr;
	soundAtts.G1 = nullptr;
	soundAtts.reverb = nullptr;
}

