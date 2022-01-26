#ifndef KINEMATIC_H
#define KINEMATIC_H

#include "Vector.h"
#include "components/Transform.h"
#include "SteeringOutput.h"

class GameObject;

using namespace MATH;

//Holds all kinematic AI algorithms
namespace Kinematic	{

	class KinematicSteeringOutput : public SteeringOutput	{
	public:
		Vec3 velocity, rotation;
		
		KinematicSteeringOutput();
		KinematicSteeringOutput(Vec3 velocity_, Vec3 rotation_);
		virtual void Update(float deltaTime, GameObject* aiObject_) override;
		
	};
	
	class KinematicSeek	{
	private:
		GameObject* aiObject;
		Transform target;

	public:
		KinematicSeek(GameObject* aiObject_, const Transform& target_);
		~KinematicSeek();
		bool getSteering();
		
	};

	class KinematicArrive	{
	private:
		GameObject* aiObject;
		Transform target;
		
		float radius;
		float timeToTarget; //generally lest than 1 second

	public:
		KinematicArrive(GameObject* aiObject_, const Transform& target_, const float radius_, const float timeToTarget_);
		~KinematicArrive();
		bool getSteering();
	};
}



#endif
