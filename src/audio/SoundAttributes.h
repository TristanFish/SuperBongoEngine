#ifndef SOUNDATTRIBUTES_H
#define SOUNDATTRIBUTES_H

#include <vector>

#include <fmod/fmod.h>
#include <fmod/fmod.hpp>
#include <fmod/fmod_common.h>

//struct for FMOD tools to be accessed through, each audio source should have one.
struct SoundAttributes
{
	FMOD::Channel* C1 = nullptr;
	FMOD::ChannelGroup* G1 = nullptr;
	FMOD::Reverb3D* reverb = nullptr;
	//Load this up with whatever sounds you want the object to play
	std::vector <FMOD::Sound> sounds;
	
};
#endif
