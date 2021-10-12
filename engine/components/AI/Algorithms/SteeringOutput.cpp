#include "SteeringOutput.h"

SteeringOutput::SteeringOutput()	{
	iVelocity = Vec3(0.0f);
	iRotation = Vec3(0.0f);
}

SteeringOutput::SteeringOutput(Vec3 iVelocity_, Vec3 iRotation_)	{
	iVelocity = iVelocity_;
	iRotation = iRotation_;
}


