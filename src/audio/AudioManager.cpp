#include "AudioManager.h"


void AudioManager::CreateChannelGroup()
{
	//A channel is a string of audio, you can probably guess what a group is
	system->createChannelGroup("ChannelGroup1",&G1);
}

void AudioManager::InitAudioManager()
{
	FMOD_RESULT result;
	result = FMOD::System_Create(&system);
	if (result != FMOD_OK) {
		std::cout << "FMOD system unable to create (%d)" << result << std::endl;
	}

	//This actually initializes FMOD, first parameter is just a common amount of channels on a modern device.
	result = system->init(512, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK) {
		std::cout << "FMOD did not init properly (%d)" << result << std::endl;
	}
}

void AudioManager::CreateSound(const char* filename)
{
	FMOD_CREATESOUNDEXINFO tempSEI;
	system->createSound("sounds/tumbelweed.wav", FMOD_CREATESAMPLE, &tempSEI, &newSound);
}

void AudioManager::PlaySound(const char* filename)
{
	CreateSound(filename);
	CreateChannelGroup();
	system->playSound(newSound, G1, false, &C1);
}

AudioManager::~AudioManager()
{
	 delete(system); 
	 delete(newSound); 
	 delete(G1); 
	 delete(C1); 
}