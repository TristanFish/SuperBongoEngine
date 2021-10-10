#ifndef DYNAMIC_H
#define DYNAMIC_H

#include "Vector.h"

class GameObject;


using namespace MATH;

namespace Dynamic {

	class DynamicSteeringOutput	{
	public:
		Vec3 linearAccel;
		Vec3 angularAccel;

		DynamicSteeringOutput();
		DynamicSteeringOutput(Vec3 linearAccel_, Vec3 angularAccel_);

		//DynamicSteeringOutput + DynamicSteeringOutput operator
		inline const DynamicSteeringOutput operator + (const DynamicSteeringOutput& v) const {
			return DynamicSteeringOutput(linearAccel + v.linearAccel, angularAccel + v.angularAccel);
		}
		//DynamicSteeringOutput += DynamicSteeringOutput operator
		inline DynamicSteeringOutput& operator += (const DynamicSteeringOutput& v) {
			linearAccel += v.linearAccel;
			angularAccel += v.angularAccel;
			return *this;
		}
	};

	class DynamicSeek	{
	protected:
		GameObject* aiObject;
		GameObject* target;
	public:
		DynamicSeek(GameObject* aiObject_, GameObject* target_);
		
		//DynamicSeeking 
		bool getSteering();
	};

	class DynamicFlee	{
	private:
		GameObject* aiObject;
		GameObject* target;
	public:
		DynamicFlee(GameObject* aiObject_, GameObject* target_);

		//DynamicFleeing 
		bool getSteering();
	};

	class DynamicArrive {
	private:
		GameObject* aiObject;
		GameObject* target;
		float arrivalRadius, slowRadius, timeToTarget;
	public:
		DynamicArrive(GameObject* aiObject_, GameObject* target_, float arrivalRadius_, float slowRadius_, float timeToTarget_);

		//DynamicArriving
		bool getSteering();

	};

	class DynamicAlign {
	protected:
		GameObject* aiObject;
		GameObject* target;
		float arrivalRadius, slowRadius, timeToTarget;
	public:
		DynamicAlign(GameObject* aiObject_, GameObject* target_, float arrivalRadius_, float slowRadius_, float timeToTarget_);

		//DynamicAlign
		bool getSteering();
	};

	//insert Dinamic Look where ur going


	class DynamicObstacleAvoidance : protected DynamicSeek {
	private:
		float avoidDistance, lookAhead;

	public:
		DynamicObstacleAvoidance(GameObject* aiObject_, GameObject* target_, float avoidDistance_, float lookAhead_);

		bool getSteering();

	};
	
}


#endif // !DYNAMIC_H
