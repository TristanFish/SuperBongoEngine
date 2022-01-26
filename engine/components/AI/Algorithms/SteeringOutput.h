#ifndef STEERINGOUTPUT_H
#define STEERINGOUTPUT_H

#include"Vector.h"

class GameObject;

using namespace MATH;

//Ai steeringOutput abstract class/interface, includes "Vector.h"
class SteeringOutput {
public:

	SteeringOutput();
	virtual void Update(float deltaTime, GameObject* aiObject_) {}
};


#endif // !1

