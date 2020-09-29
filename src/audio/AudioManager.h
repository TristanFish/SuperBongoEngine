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
	//Creating a new instance of this class will nor work. The get function belows initializes the class.
	//Call other functions found in Audio Manager but calling AudioManager::Get()."whatever function"

	static AudioManager* Get() { static AudioManager s_AudioManager;  
											 return &s_AudioManager; }

	void Update();
	void CreateAndPlaySound(const char* filename);
	void MonitorChannel(FMOD::Channel c);

	FMOD::ChannelGroup* G1 = nullptr;

	//FMOD runs off a single system pointer below, most of the initialization is behind the scenes in regards to hooking into your sound device/card 
	FMOD::System* system = nullptr;
		
	~AudioManager();

private:

	std::map <std::string, std::string> sounds;
	//std::vector <AudioSourceComponent> audioSources;

	FMOD::Sound* newSound = nullptr;
	FMOD::Channel* C1 = nullptr;
	FMOD::Reverb3D* reverb = nullptr;

	FMOD_VECTOR pos {};
	FMOD_VECTOR audioSourcePos{};
	FMOD_VECTOR forward{0.0f, 0.0f, 1.0f};
	FMOD_VECTOR up{0.0f ,1.0f, 0.0f};

	
	AudioManager();
	AudioManager(const AudioManager&) = delete;

	//Just to keep track of all of our audio sources
	void AddAudioSource(AudioSourceComponent& newComponent);

	void InitAudioManager();
	void CreateChannelGroup();
	void SetListenerPos(MATH::Vec3 pos);
	void CreateSounds();
	void Create3DReverbAttributes(FMOD_VECTOR pos);
	void Create3DReverb();
	void CreateAndSetPan(float pan);
	void SetAudioSourcePos(MATH::Vec3 sourcepos);

	//Please keep all sounds keys in all lower case so we don't have to debug dumb stuff
	//Add any sounds you want in this function, just follow suit.
	void LoadSoundMap();

};
#endif



//TODO
//Set up better way to handle loading sounds (store strings in a map of file paths and load everything into sound objects proably) 
//Set audio distance units to our engine's
//Store all audio sources in audio source vector
//Create event hooks for audio sources to play
//Group all audio sources