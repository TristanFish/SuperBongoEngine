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
		virtual void Update(float deltaTime, std::shared_ptr<GameObject> aiObject_) override;
		
	};
	
	class KinematicSeek	{
	private:
		std::shared_ptr<GameObject> aiObject;
		Transform target;

	public:
		KinematicSeek(std::shared_ptr<GameObject> aiObject_, const Transform& target_);
		~KinematicSeek();
		KinematicSteeringOutput getSteering();
		
	};

	class KinematicArrive	{
	private:
		std::shared_ptr<GameObject> aiObject;
		Transform target;
		
		float radius;
		float timeToTarget; //generally lest than 1 second

	public:
		KinematicArrive(std::shared_ptr<GameObject> aiObject_, const Transform& target_, const float radius_, const float timeToTarget_);
		~KinematicArrive();
		KinematicSteeringOutput getSteering();
	};
	
}



#endif
