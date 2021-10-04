#include "Kinematic.h"

#include "components/GameObject.h"
#include "components/AI/AIComponent.h"

using namespace Kinematic;


//Kinematic Steering Output
KinematicSteeringOutput::KinematicSteeringOutput()	{
	velocity = Vec3(0.0f);
	rotation = Vec3(0.0f);
}

KinematicSteeringOutput::KinematicSteeringOutput(Vec3 velocity_, Vec3 rotation_) {
	velocity = velocity_;
	rotation = rotation_;
}

//Kinematic Seek
#pragma region KinematicSeek
KinematicSeek::KinematicSeek(GameObject* aiObject_, GameObject* target_)	{
	aiObject = aiObject_;
	target = target_;
}

KinematicSeek::~KinematicSeek()	{

}

bool KinematicSeek::getSteering()	{
	//Checks if aiObject has the required components to function
	if(!aiObject->HasComponent<AIComponent>())	{
		EngineLogger::Error(aiObject->GetName() + " does not have an AIComponent. KinematicSteering has failed", 
			"Kinematic.cpp", __LINE__);

		return false;
	}
	
	AIComponent* objectAIComp = aiObject->GetComponent<AIComponent>();

	//change this to stack
	KinematicSteeringOutput result = KinematicSteeringOutput();
	
	
	//Gets direction to the target
	result.velocity = target->transform.GetPosition() - aiObject->transform.GetPosition();
	
	//sets velocity at max possible speed
	result.velocity = VMath::normalize(result.velocity);
	result.velocity = objectAIComp->GetMaxSpeed() * result.velocity;

	//sets the rotation to be in the direction of the velocity - WIP this needs to be tested
	aiObject->SetRotation(result.velocity);

	result.rotation = Vec3(0.0f);
	objectAIComp->SetSteering(result);
	
	return true;
}
#pragma endregion 

//Kinematic Arrive
#pragma region KinematicArrive
KinematicArrive::KinematicArrive(GameObject* aiObject_, GameObject* target_, float radius_, float timeToTarget_)	{
	aiObject = aiObject_;
	target = target_;
	radius = radius_;
	timeToTarget = timeToTarget_;
}

KinematicArrive::~KinematicArrive()	{

}

bool KinematicArrive::getSteering()	{
	//Checks if aiObject has the required components to function
	if (!aiObject->HasComponent<AIComponent>()) {
		EngineLogger::Error(aiObject->GetName() + " does not have an AIComponent. KinmaticArrive has failed",
			"Kinematic.cpp", __LINE__);

		//returns 0s when failed
		return false;
	}

	AIComponent* objectAIComp = aiObject->GetComponent<AIComponent>();
	
	KinematicSteeringOutput result = KinematicSteeringOutput();

	//Get direction to target
	result.velocity = target->transform.GetPosition() - aiObject->transform.GetPosition();
	//check if in radius
	if (VMath::mag(result.velocity) < radius)	{
		//does not steer as it is in radius
		result.velocity = Vec3(0.0f);
		result.rotation = Vec3(0.0f);
		
		return false;
	}
	//move to target in timeToTarget
	result.velocity = result.velocity / timeToTarget;

	//rotate to target - WIP needs to be tested
	aiObject->SetRotation(result.velocity);

	result.rotation = Vec3(0.0f);
	
	objectAIComp->SetSteering(result);
	
	return true;
}
#pragma endregion