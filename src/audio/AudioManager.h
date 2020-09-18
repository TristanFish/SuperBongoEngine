#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <iostream>

#include <fmod/fmod.h>
#include <fmod/fmod.hpp>
#include <fmod/fmod_common.h>
#include <fmod/fmod_errors.h>
class AudioManager
{
public:
	~AudioManager();
	AudioManager();

	void Update();
	void InitAudioManager();
	void PlaySound(const char* filename);
	void MonitorChannel(FMOD::Channel c);

	FMOD::Channel* C1 = nullptr;
private:

	//FMOD runs off a single system pointer below, most of the initialization is behind the scenes in regards to hooking into your sound device/card 
	FMOD::System* system = nullptr;
	FMOD::Sound* newSound = nullptr;
	FMOD::ChannelGroup* G1 = nullptr;
	FMOD::Reverb3D* reverb = nullptr;

	FMOD_VECTOR pos { 0.0f, 5.0f, 0.0f };

	void CreateChannelGroup();
	void CreateSound(const char* filename);
	void Create3DReverbAttributes(FMOD_VECTOR pos);
	void Create3DReverb();

	float dBToVolume(float dB);
	float VolumeTodB(float vol);
	
};
#endif
