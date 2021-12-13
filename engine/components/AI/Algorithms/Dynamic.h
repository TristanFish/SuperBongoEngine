#ifndef DYNAMIC_H
#define DYNAMIC_H

#include "Vector.h"
#include "components/Transform.h"
#include "SteeringOutput.h"

class GameObject;
using namespace MATH;

namespace Dynamic {

	class DynamicSteeringOutput : public SteeringOutput	{
	public:
		Vec3 linearAccel, angularAccel;

		DynamicSteeringOutput();
		DynamicSteeringOutput(Vec3 linearAccel_, Vec3 angularAccel_);
		virtual void Update(const float deltaTime, GameObject* aiObject_) override;

		////DynamicSteeringOutput + DynamicSteeringOutput operator
		//inline const DynamicSteeringOutput operator + (const DynamicSteeringOutput& v) const {
		//	return DynamicSteeringOutput(linearAccel + v.linearAccel, angularAccel + v.angularAccel);
		//}
		////DynamicSteeringOutput += DynamicSteeringOutput operator
		//inline DynamicSteeringOutput& operator += (const DynamicSteeringOutput& v) {
		//	linearAccel += v.linearAccel;
		//	angularAccel += v.angularAccel;
		//	return *this;
		//}
	};

	class DynamicSeek	{
	protected:
		GameObject* aiObject;
		Transform target;
	public:
		DynamicSeek(GameObject* aiObject_, const Transform& target_);
		
		//DynamicSeeking 
		bool getSteering();
	};

	class DynamicFlee	{
	private:
		GameObject* aiObject;
		Transform target;
	public:
		DynamicFlee(GameObject* aiObject_, const Transform& target_);

		//DynamicFleeing 
		bool getSteering();
	};

	class DynamicArrive {
	private:
		GameObject* aiObject;
		Transform target;
		float arrivalRadius, slowRadius, timeToTarget;
	public:
		DynamicArrive(GameObject* aiObject_, const Transform& target_, const float arrivalRadius_, const float slowRadius_, float timeToTarget_);

		//DynamicArriving
		bool getSteering();

	};

	class DynamicAlign {
	protected:
		GameObject* aiObject;
		Transform target;
		float arrivalRadius, slowRadius, timeToTarget;
	public:
		DynamicAlign(GameObject* aiObject_, const Transform& target_, const float arrivalRadius_, const float slowRadius_, float timeToTarget_);

		//DynamicAlign
		bool getSteering();
	};

	//insert Dinamic Look where ur going


	class DynamicObstacleAvoidance : protected DynamicSeek {
	private:
		float avoidDistance, lookAhead;

	public:
		DynamicObstacleAvoidance(GameObject* aiObject_, const Transform& target_, const float avoidDistance_, const float lookAhead_);

		bool getSteering();

	};
	
}


#endif // !DYNAMIC_H
