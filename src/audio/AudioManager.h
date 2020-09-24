#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <iostream>

#include <fmod/fmod.h>
#include <fmod/fmod.hpp>
#include <fmod/fmod_common.h>
#include <fmod/fmod_errors.h>
#include <math/VMath.h>
class AudioManager
{
public:
	~AudioManager();
	AudioManager();

	void Update();
	void InitAudioManager();
	void CreateAndPlaySound(const char* filename);
	void MonitorChannel(FMOD::Channel c);

	FMOD::Channel* C1 = nullptr;
private:

	//FMOD runs off a single system pointer below, most of the initialization is behind the scenes in regards to hooking into your sound device/card 
	FMOD::System* system = nullptr;
	FMOD::Sound* newSound = nullptr;
	FMOD::ChannelGroup* G1 = nullptr;
	FMOD::Reverb3D* reverb = nullptr;

	//Make this camera pos
	FMOD_VECTOR pos {};
	FMOD_VECTOR audioSourcePos{};

	//For camera position later
	float currentPan;

	void CreateChannelGroup();
	void SetListenerPos(MATH::Vec3 pos);
	void CreateSound(const char* filename);
	void Create3DReverbAttributes(FMOD_VECTOR pos);
	void Create3DReverb();
	void CreateAndSetPan(float pan);
	void SetAudioSourcePos(MATH::Vec3 sourcepos);

	//Utility
	float dBToVolume(float dB);
	float VolumeTodB(float vol);
	FMOD_VECTOR Vec3ToFMODVec(MATH::Vec3);
	
};
#endif

//TODO
//Set up set3Dspread based on orientation of object and player. When sound occurs draw a line from sound source and player for directional sounds
//Inherit from component, seperate class for listener and sound source? Listener could be both.
//Set audio distance units to our engine's