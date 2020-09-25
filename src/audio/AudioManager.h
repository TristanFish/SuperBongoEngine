#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <iostream>
#include <vector>

#include <fmod/fmod.h>
#include <fmod/fmod.hpp>
#include <fmod/fmod_common.h>
#include <fmod/fmod_errors.h>
#include <math/VMath.h>

class AudioSourceComponent;

class AudioManager
{
public:
	//Creating a new instance won't work anymore, must be stored in a reference. 
	AudioManager(const AudioManager&) = delete;
	AudioManager();
	~AudioManager();


	void Update();
	void InitAudioManager();
	void CreateAndPlaySound(const char* filename);
	void MonitorChannel(FMOD::Channel c);

	FMOD::ChannelGroup* G1 = nullptr;
	FMOD::System* system = nullptr;

	static AudioManager& Get() { return s_AudioManager; }
	

private:

	static AudioManager s_AudioManager;

	std::vector <AudioSourceComponent> audioSources;
	//FMOD runs off a single system pointer below, most of the initialization is behind the scenes in regards to hooking into your sound device/card 
	FMOD::Sound* newSound = nullptr;
	FMOD::Channel* C1 = nullptr;
	FMOD::Reverb3D* reverb = nullptr;

	//Make this camera pos
	FMOD_VECTOR pos {};
	FMOD_VECTOR audioSourcePos{};
	FMOD_VECTOR forward{0.0f, 0.0f, 1.0f};
	FMOD_VECTOR up{0.0f ,1.0f, 0.0f};

	//Just to keep track of all of our audio sources
	void AddAudioSource(AudioSourceComponent& newComponent);

	void CreateChannelGroup();
	void SetListenerPos(MATH::Vec3 pos);
	void CreateSound(const char* filename);
	void Create3DReverbAttributes(FMOD_VECTOR pos);
	void Create3DReverb();
	void CreateAndSetPan(float pan);
	void SetAudioSourcePos(MATH::Vec3 sourcepos);

	//Utility
	
};
#endif

struct AudioUtility {
	inline static float dBToVolume(float dB);
	inline static float VolumeTodB(float vol);
	inline static FMOD_VECTOR Vec3ToFMODVec(MATH::Vec3);
	inline static FMOD_VECTOR& Vec3ToFMODVecR(MATH::Vec3& vec);
};

//TODO
//Set up set3Dspread based on orientation of object and player. When sound occurs draw a line from sound source and player for directional sounds.
//Inherit from component, seperate class for listener and sound source? Listener could be both.
//Set up better way to handle loading sounds (store strings of file paths and load everything into sound objects proably) 
//Set audio distance units to our engine's