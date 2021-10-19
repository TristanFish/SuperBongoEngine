#include "Kinematic.h"

#include "components/GameObject.h"
#include "components/AI/AIComponent.h"

using namespace Kinematic;


//Kinematic Steering Output
KinematicSteeringOutput::KinematicSteeringOutput() : SteeringOutput() {
	velocity = Vec3(0.0f);
	rotation = Vec3(0.0f);
}

KinematicSteeringOutput::KinematicSteeringOutput(Vec3 velocity_, Vec3 rotation_) {
	velocity = velocity_;
	rotation = rotation_;
}

void KinematicSteeringOutput::Update(float deltaTime, GameObject* aiObject_)	{

	if (aiObject_->HasComponent<RigidBody3D>() && aiObject_->HasComponent<AIComponent>()) {
		
		aiObject_->GetComponent<RigidBody3D>()->SetAngVelocity(rotation); // if this causes issues old code was:  transform.rotation += rotation * deltaTime;
		if (VMath::mag(velocity) > aiObject_->GetComponent<AIComponent>()->GetMaxSpeed()) {
			velocity = VMath::normalize(velocity) * aiObject_->GetComponent<AIComponent>()->GetMaxSpeed();
		}
		aiObject_->GetComponent<RigidBody3D>()->SetVelocity(velocity);
	}

}

//Kinematic Seek
#pragma region KinematicSeek
KinematicSeek::KinematicSeek(GameObject* aiObject_, const Transform& target_)	{
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

	//change this to stack
	KinematicSteeringOutput result = KinematicSteeringOutput();
	
	
	//Gets direction to the target
	result.velocity = target.GetPosition() - aiObject->transform.GetPosition();
	
	//sets velocity at max possible speed
	result.velocity = VMath::normalize(result.velocity);
	result.velocity = aiObject->GetComponent<AIComponent>()->GetMaxSpeed() * result.velocity;

	//sets the rotation to be in the direction of the velocity - WIP this needs to be tested
	aiObject->SetRotation(result.velocity);

	result.rotation = Vec3(0.0f);
	aiObject->GetComponent<AIComponent>()->SetSteering(&result);
	
	return true;
}
#pragma endregion 

//Kinematic Arrive
#pragma region KinematicArrive
KinematicArrive::KinematicArrive(GameObject* aiObject_, const Transform& target_, const float radius_, const float timeToTarget_)	{
	aiObject = aiObject_;
	target = target_;
	radius = radius_;
	if (timeToTarget_ > VERY_SMALL)	timeToTarget = timeToTarget_;
	else timeToTarget = 0.1f;
}

KinematicArrive::~KinematicArrive()	{

}

bool KinematicArrive::getSteering()	{
	//Checks if aiObject has the required components to function
	if (!aiObject->HasComponent<AIComponent>()) {
		EngineLogger::Error(aiObject->GetName() + " does not have an AIComponent. KinmaticArrive has failed",
			"Kinematic.cpp", __LINE__);

		return false;
	}
	
	KinematicSteeringOutput result = KinematicSteeringOutput();

	//Get direction to target
	result.velocity = target.GetPosition() - aiObject->transform.GetPosition();
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
	//this is what we think we need (ish)
	//aiObject->transform.rotationMatrix = MMath::lookAt(aiObject->transform.GetPosition(), result.velocity, Vec3(0.0f, 1.0f, 0.0f));

	//this is what it is rn (wrong)
	aiObject->SetRotation(result.velocity);
	
	result.rotation = Vec3(0.0f);
	
	aiObject->GetComponent<AIComponent>()->SetSteering(&result);
	
	return true;
}
#pragma endregion