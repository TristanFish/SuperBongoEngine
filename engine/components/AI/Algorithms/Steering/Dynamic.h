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
		virtual void Update(const float deltaTime, std::shared_ptr<GameObject> aiObject_) override;

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
		std::shared_ptr<GameObject> aiObject;
		Transform target;
	public:
		DynamicSeek(std::shared_ptr<GameObject> aiObject_, const Transform& target_);
		
		//DynamicSeeking 
		DynamicSteeringOutput getSteering();
	};

	class DynamicFlee	{
	private:
		std::shared_ptr<GameObject> aiObject;
		Transform target;
	public:
		DynamicFlee(std::shared_ptr<GameObject> aiObject_, const Transform& target_);

		//DynamicFleeing 
		DynamicSteeringOutput getSteering();
	};

	class DynamicArrive {
	private:
		std::shared_ptr<GameObject> aiObject;
		Transform target;
		float arrivalRadius, slowRadius, timeToTarget;
	public:
		DynamicArrive(std::shared_ptr<GameObject> aiObject_, const Transform& target_, const float arrivalRadius_, const float slowRadius_, float timeToTarget_);

		//DynamicArriving
		DynamicSteeringOutput getSteering();

	};

	class DynamicAlign {
	protected:
		std::shared_ptr<GameObject> aiObject;
		Transform target;
		float arrivalRadius, slowRadius, timeToTarget;
	public:
		DynamicAlign(std::shared_ptr<GameObject> aiObject_, const Transform& target_, const float arrivalRadius_, const float slowRadius_, float timeToTarget_);

		//DynamicAlign
		DynamicSteeringOutput getSteering();
	};

	//insert Dinamic Look where ur going


	class DynamicObstacleAvoidance : protected DynamicSeek {
	private:
		float avoidDistance, lookAhead;

	public:
		DynamicObstacleAvoidance(std::shared_ptr<GameObject> aiObject_, const Transform& target_, const float avoidDistance_, const float lookAhead_);

		DynamicSteeringOutput getSteering();

	};
	
}


#endif // !DYNAMIC_H
