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
	result = system->init(512, FMOD_INIT_3D_RIGHTHANDED, 0);
	if (result != FMOD_OK) {
		std::cout << "FMOD did not init properly" << result << "\n" << FMOD_ErrorString(result) << std::endl;
		exit(-1);
	}
}

void AudioManager::CreateSound(const char* filename)
{
	FMOD_RESULT r;
	FMOD_CREATESOUNDEXINFO tempSEI;
	r = system->createSound(filename, FMOD_3D, nullptr, &newSound);
	std::cout << r << FMOD_ErrorString(r)<< std::endl;
}

void AudioManager::Create3DReverbAttributes(FMOD_VECTOR position)
{
	FMOD_VECTOR pos = position;
	float mindist = 1.0f;
	float maxdist = 30.0f;
	reverb->set3DAttributes(&pos, mindist, maxdist);

	FMOD_VECTOR listenerPos = { -5.0f, 5.0f, -1.0f };
	system->set3DListenerAttributes(0, &listenerPos, 0, 0, 0);
}

void AudioManager::Create3DReverb()
{
	FMOD_RESULT r;
	r = system->createReverb3D(&reverb);
	FMOD_REVERB_PROPERTIES prop = FMOD_PRESET_FOREST;
	reverb->setProperties(&prop);
	Create3DReverbAttributes(pos);
}

float AudioManager::dBToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

float AudioManager::VolumeTodB(float vol)
{
	return 20.0f * log10f(vol);
}

void AudioManager::PlaySound(const char* filename)
{
	CreateSound(filename);
	CreateChannelGroup();
	Create3DReverb();
	system->playSound(newSound, nullptr, false, &C1);
}

AudioManager::~AudioManager()
{
	system->release();
	system = nullptr;
	newSound = nullptr;
	G1 = nullptr;
	C1 = nullptr;
	
}

AudioManager::AudioManager() : system(nullptr)
{
	
}
