#ifndef AUDIOUTILITY_H
#define AUDIOUTILITY_H

#include <fmod/fmod.hpp>
#include <fmod/fmod.h>
#include <fmod/fmod_common.h>
#include <math/VMath.h>

class AudioUtility {
public:
	AudioUtility();
	static float dBToVolume(float dB);
	static float VolumeTodB(float vol);
	static FMOD_VECTOR Vec3ToFMODVec(MATH::Vec3);
	static FMOD_VECTOR Vec3ToFMODVecR(MATH::Vec3& vec);
};
#endif
