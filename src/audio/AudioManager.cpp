#include "AudioManager.h"



AudioManager::AudioManager() : system(nullptr)
{

}

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

void AudioManager::SetListenerPos(MATH::Vec3 pos)
{
	this->pos = Vec3ToFMODVec(pos);
	system->set3DListenerAttributes(0, &this->pos, 0, 0, 0);
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
	if (r != FMOD_OK) {
		std::cout << r << FMOD_ErrorString(r) << std::endl;
	}
}

void AudioManager::Create3DReverbAttributes(FMOD_VECTOR position)
{
	FMOD_VECTOR pos = position;
	float mindist = 1.0f;
	float maxdist = 30.0f;
	reverb->set3DAttributes(&pos, mindist, maxdist);

	FMOD_VECTOR listenerPos = { -5.0f, 5.0f, -1.0f };
	
}

void AudioManager::Create3DReverb()
{
	//FMOD uses properties/attributes of a specific struct to determine how the property sounds, if you look at a preset you'll see-
	//a series of arbitrary numbers that determine which DSP effects are altered

	FMOD_RESULT r;
	r = system->createReverb3D(&reverb);
	FMOD_REVERB_PROPERTIES prop = FMOD_PRESET_CAVE;
	reverb->setProperties(&prop);
	Create3DReverbAttributes(audioSourcePos);
	
	//CreateAndSetPan(-1);
}

void AudioManager::CreateAndSetPan(float pan)
{
	currentPan = pan;
	C1->setPan(pan);
	C1->set3DLevel(1);
}

void AudioManager::SetAudioSourcePos(MATH::Vec3 sourcePos)
{
	audioSourcePos = Vec3ToFMODVec(sourcePos);
	C1->set3DAttributes(&audioSourcePos, NULL);
	G1->set3DAttributes(&audioSourcePos, NULL);
}


void AudioManager::CreateAndPlaySound(const char* filename)
{
	CreateSound(filename);
	CreateChannelGroup();
	SetListenerPos(MATH::Vec3(0.0f,0.0f,0.0f));
	SetAudioSourcePos(MATH::Vec3(15.0f, 5.0f, 0.0f));
	Create3DReverb();
	Update();
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

///////////////////////////
// Just utility functions//
///////////////////////////
float AudioManager::dBToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

float AudioManager::VolumeTodB(float vol)
{
	return 20.0f * log10f(vol);
}

FMOD_VECTOR AudioManager::Vec3ToFMODVec(MATH::Vec3 vec)
{
	FMOD_VECTOR temp;
	temp.x = vec.x;
	temp.y = vec.y;
	temp.z = vec.z;
	return temp;
}
