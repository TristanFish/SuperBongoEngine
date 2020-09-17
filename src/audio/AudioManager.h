#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <iostream>

#include <fmod/fmod.h>
#include <fmod/fmod.hpp>
#include <fmod/fmod_common.h>
class AudioManager
{
public:
	~AudioManager();

	void InitAudioManager();
	void PlaySound(const char* filename);

private:


	//FMOD runs off a single system pointer below, most of the initialization is behind the scenes in regards to hooking into your sound device/card 
	FMOD::System* system = nullptr;
	FMOD::Sound* newSound;
	FMOD::ChannelGroup* G1;
	FMOD::Channel* C1;

	void CreateChannelGroup();
	void CreateSound(const char* filename);
};
#endif
