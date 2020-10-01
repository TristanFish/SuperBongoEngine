#include "AudioSourceComponent.h"

void AudioSourceComponent::Init(GameObject* g)
{
	soundAtts.C1 = nullptr;
	gameobject = nullptr;
	sound1 = nullptr;
	audioSystemRef = nullptr;

	gameobject = g;
	gameObjectPosition = g->transform.pos;

	audioSystemRef = AudioManager::Get()->system;
	FMODGamePos =  AudioUtility::Vec3ToFMODVec(gameObjectPosition);

	AudioManager::Get()->CreateChannelGroup("Channel Group 1", soundAtts.G1);
	LoadSoundVector();
}

void AudioSourceComponent::Update(const float deltaTime)
{
//Set a check here to see if sound is ready to play? Then update position
}


void AudioSourceComponent::PlaySound(FMOD::Sound* sound)
{
	audioSystemRef->playSound(sound, soundAtts.G1, true, &soundAtts.C1);
	SetAudioPosition();
	soundAtts.C1->setChannelGroup(soundAtts.G1);
	soundAtts.C1->setPaused(false);
}

FMOD::Sound* AudioSourceComponent::GetFreshSoundBuffer()
{
	//Return the first sound buffer that is not full
	for (int i = 0; i < sounds.size(); i++) {
		if (sounds[i] != nullptr) {
			return sounds[i];
		}
	}
	std::cout << "no free sound buffers available check to make sure you havent filled the sounds vector" << std::endl;
	return nullptr;
}

FMOD::Sound* AudioSourceComponent::LoadSound(const char* soundName, FMOD::Sound* soundBuffer)
{
	soundBuffer = AudioManager::Get()->RetrieveSoundObject(soundName);
	if (soundBuffer == nullptr) {
		std::cout << "Sound buffer not loaded. Make sure buffer is valid and available." << std::endl;
		return NULL;
	}
	return soundBuffer;
}

FMOD::Sound* AudioSourceComponent::GetSound(int soundNumber)
{
	auto temp = sounds[soundNumber];
	if (temp == nullptr) {
		std::cout << "invalid sound element" << std::endl;
	}
	return temp;
}


void AudioSourceComponent::SetAudioPosition()
	
{
	FMOD_RESULT r;
	//Gotta convert to FMODVECTOR, its no problem as long as the vector is always a vec3
	r = soundAtts.C1->set3DAttributes(&FMODGamePos, NULL);
	if (r != FMOD_OK) {
		std::cout << r << FMOD_ErrorString(r) << "-------"<< "Audio Component did not set 3D attributes, game object position unavailable"<< std::endl;
	}
}

void AudioSourceComponent::LoadSoundVector()
{
	//You'll have to manually enter sound objects in here if you want more, for now each audiosource can have 3 sounds
	sounds.emplace_back(sound1);
	sounds.emplace_back(sound2);
	sounds.emplace_back(sound3);
}

void AudioSourceComponent::Render(){}
void AudioSourceComponent::HandleEvents(const SDL_Event& event){}

AudioSourceComponent::~AudioSourceComponent()
{
	gameobject = nullptr; 
	sound1 = nullptr;
	audioSystemRef = nullptr;
}

