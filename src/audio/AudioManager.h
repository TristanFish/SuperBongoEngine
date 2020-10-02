#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <iostream>
#include <vector>
#include <map>

#include <fmod/fmod.h>
#include <fmod/fmod.hpp>
#include <fmod/fmod_common.h>
#include <fmod/fmod_errors.h>
#include <math/VMath.h>
#include <audio/AudioUtility.h>

class AudioSourceComponent;

class AudioManager
{
public:
	//Creating a new instance of this class will not work. The get function belows initializes the class.
	//Call other functions found in Audio Manager but calling AudioManager::Get()."whatever function"

	static AudioManager* Get() { static AudioManager s_AudioManager;  
											 return &s_AudioManager; }

	void Update();
	void CreateAndPlaySound(const char* filename);
	void MonitorChannel(FMOD::Channel c);
	void CreateChannelGroup(const char* groupName, FMOD::ChannelGroup* channelGroup);

	FMOD::Sound* RetrieveSoundObject(std::string soundName);
	FMOD::System* system = nullptr;
	FMOD::ChannelGroup* G1 = nullptr;
	FMOD::Channel* C1 = nullptr;

		
	~AudioManager();

private:

//Sound bank
	#pragma region sounds
	FMOD::Sound* newSound = nullptr;
	FMOD::Sound* leafCrunch = nullptr;
#pragma endregion 
		
	FMOD::Reverb3D* reverb = nullptr;
	
	std::vector <std::string> soundPaths;
	std::vector <FMOD::Sound*> sounds;
	std::map <std::string, FMOD::Sound*> soundPairs;
	
	AudioManager();
	AudioManager(const AudioManager&) = delete;

	//Just to keep track of all of our audio sources
	void AddAudioSource(AudioSourceComponent& newComponent);

	void InitAudioManager();
	void CreateSounds();
	void Create3DReverbAttributes(FMOD_VECTOR pos);
	void Create3DReverb();
	void CreateAndSetPan(float pan);
	//void SetAudioSourcePos(MATH::Vec3& sourcepos);

	//This is where we load sounds, provide a new buffer and file path for every sound
	void LoadSounds();

	
	

};
#endif


//TODO
//Load sounds into vector and have create sound iterate through all sounds
//Set up better way to handle loading sounds (store strings in a map of file paths and load everything into sound objects proably) 
//Set audio distance units to our engine's
//Store all audio sources in audio source vector
//Create event hooks for audio sources to play
//Group all audio sources