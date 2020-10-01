#include "AudioUtility.h"


AudioUtility::AudioUtility()
{
}

float AudioUtility::dBToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

float AudioUtility::VolumeTodB(float vol)
{
	return 20.0f * log10f(vol);
}

FMOD_VECTOR AudioUtility::Vec3ToFMODVec(MATH::Vec3 vec)
{
	FMOD_VECTOR temp;
	temp.x = vec.x;
	temp.y = vec.y;
	temp.z = vec.z;
	return temp;
}

FMOD_VECTOR& AudioUtility::Vec3ToFMODVecR(MATH::Vec3& vec)
{
	FMOD_VECTOR temp;
	temp.x = vec.x;
	temp.y = vec.y;
	temp.z = vec.z;
	return temp;

}