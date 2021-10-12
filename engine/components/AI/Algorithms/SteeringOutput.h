#ifndef STEERINGOUTPUT_H
#define STEERINGOUTPUT_H

#include"Vector.h"

class GameObject;

using namespace MATH;

//Ai steeringOutput abstract class/interface, includes "Vector.h"
class SteeringOutput {
public:
	//linearAccel or Veloctiy
	Vec3 iVelocity;
	//angularAccel or rotation
	Vec3 iRotation;

	SteeringOutput();
	SteeringOutput(Vec3 iVelocity_, Vec3 iRotation_);
	virtual void Update(const float deltaTime, GameObject* aiObject_) = 0;
};


#endif // !1

