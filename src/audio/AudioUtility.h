#ifndef AUDIOUTILITY_H
#define AUDIOUTILITY_H

#include <fmod/fmod.hpp>
#include <fmod/fmod.h>
#include <fmod/fmod_common.h>
#include <math/VMath.h>

struct AudioUtility {
	inline static float dBToVolume(float dB);
	inline static float VolumeTodB(float vol);
	inline static FMOD_VECTOR Vec3ToFMODVec(MATH::Vec3);
	inline static FMOD_VECTOR& Vec3ToFMODVecR(MATH::Vec3& vec);
};
#endif
