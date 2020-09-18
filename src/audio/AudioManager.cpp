#include "AudioManager.h"


void AudioManager::MonitorChannel(FMOD::Channel c)
{
	bool tb;
	c.isPlaying(&tb);
	std::cout  << tb << std::endl;
}

void AudioManager::CreateChannelGroup()
{
	//A channel is a string of audio, you can probably guess what a group is
	system->createChannelGroup("ChannelGroup1",&G1);
}

void AudioManager::Update()
{
	
	system->update();
}

void AudioManager::InitAudioManager()
{
	FMOD_RESULT result;
	result = FMOD::System_Create(&system);
	if (result != FMOD_OK) {
		std::cout << "FMOD system unable to create" << result << "\n" << FMOD_ErrorString(result) << std::endl;
		exit(-1);
	}

	//This actually initializes FMOD, first parameter is just a common amount of channels on a modern device.
	result = system->init(512, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK) {
		std::cout << "FMOD did not init properly" << result << "\n" << FMOD_ErrorString(result) << std::endl;
		exit(-1);
	}
}

void AudioManager::CreateSound(const char* filename)
{
	FMOD_RESULT r;
	FMOD_CREATESOUNDEXINFO tempSEI;
	r = system->createSound(filename, FMOD_CREATESAMPLE, nullptr, &newSound);
	std::cout << r << FMOD_ErrorString(r)<< std::endl;
}

void AudioManager::PlaySound(const char* filename)
{
	CreateSound(filename);
	CreateChannelGroup();
	system->playSound(newSound, nullptr, false, &C1);
}

AudioManager::~AudioManager()
{
	system->release();
}

AudioManager::AudioManager() : system(nullptr)
{
	
}
