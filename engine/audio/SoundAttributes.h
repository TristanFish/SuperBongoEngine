#ifndef SOUNDATTRIBUTES_H
#define SOUNDATTRIBUTES_H


#include <fmod/fmod.h>
#include <fmod/fmod.hpp>
#include <fmod/fmod_common.h>

//struct for FMOD tools to be accessed through, each audio source should have one.
struct SoundAttributes
{
	FMOD::Channel* C1;
	FMOD::ChannelGroup* G1;
	FMOD::Reverb3D* reverb;
	
};
#endif
