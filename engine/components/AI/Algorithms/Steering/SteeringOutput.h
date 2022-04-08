#ifndef STEERINGOUTPUT_H
#define STEERINGOUTPUT_H

#include "Quaternion.h"
#include"Vector.h"

class GameObject;
class Transform;
using namespace MATH;

//Ai steeringOutput abstract class/interface, includes "Vector.h"
class SteeringOutput {
public:

	SteeringOutput();
	
	//returns a rotation (rotating the Z axis for 2.5D style rotation) that would make the aiObject face a direction.
	//Can be used in both Kinematic or Dynamic Steering
	Quaternion calculateOrientation(std::shared_ptr<GameObject> aiObject_, Vec3 direction_);
	
	virtual void Update(float deltaTime, std::shared_ptr<GameObject> aiObject_) {}
};


#endif // !1

