#ifndef KINEMATIC_H
#define KINEMATIC_H

#include "Vector.h"

class GameObject;
class AIComponent;

using namespace MATH;

//Holds all kinematic AI algorithms
namespace Kinematic	{

	class KinematicSteeringOutput	{
	public:
		Vec3 velocity, rotation;
		
		KinematicSteeringOutput();
		KinematicSteeringOutput(Vec3 velocity_, Vec3 rotation_);
		
	};
	
	class KinematicSeek	{
	private:
		GameObject* aiObject;
		GameObject* target;

	public:
		KinematicSeek(GameObject* aiObject_, GameObject* target_);
		~KinematicSeek();
		bool getSteering();
		
	};

	class KinematicArrive	{
	private:
		GameObject* aiObject;
		GameObject* target;
		float radius, timeToTarget;

	public:
		KinematicArrive(GameObject* aiObject_, GameObject* target_, float radius_, float timeToTarget_);
		~KinematicArrive();
		bool getSteering();
	};
}



#endif
