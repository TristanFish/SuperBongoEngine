#include "Dynamic.h"

#include "components/GameObject.h"
#include "components/AI/AIComponent.h"

using namespace Dynamic;

DynamicSteeringOutput::DynamicSteeringOutput()	{
	linearAccel = Vec3(0.0f);
	angularAccel = Vec3(0.0f);
}

DynamicSteeringOutput::DynamicSteeringOutput(Vec3 linearAccel_, Vec3 angularAccel_)	{
	linearAccel = linearAccel_;
	angularAccel = angularAccel_;
}

#pragma region DynamicSeek
DynamicSeek::DynamicSeek(GameObject* aiObject_, GameObject* target_)	{
	aiObject = aiObject_;
	target = target_;
}

bool DynamicSeek::getSteering()	{
	//Checks if aiObject has the required components to function
	if (!aiObject->HasComponent<AIComponent>() || !aiObject->HasComponent<RigidBody3D>()) {
		EngineLogger::Error(aiObject->GetName() + " does not have either an AIComponent or a RigidBody3D. DynamicSeek has failed",
			"Dynamic.cpp", __LINE__);

		return false;
	}

	AIComponent* objectAIComp = aiObject->GetComponent<AIComponent>();
	DynamicSteeringOutput result = DynamicSteeringOutput();

	//gets direction
	result.linearAccel = target->transform.GetPosition() - aiObject->transform.GetPosition();

	result.linearAccel = VMath::normalize(result.linearAccel);
	result.linearAccel = objectAIComp->GetMaxAcceleration() * result.linearAccel;

	result.angularAccel = Vec3(0.0f);

	objectAIComp->SetDynamicSteering(result);

	return true;
}


#pragma endregion


#pragma region DynamicFlee
DynamicFlee::DynamicFlee(GameObject* aiObject_, GameObject* target_)	{
	aiObject = aiObject_;
	target = target_;
}

bool DynamicFlee::getSteering()	{
	//Checks if aiObject has the required components to function
	if (!aiObject->HasComponent<AIComponent>() || !aiObject->HasComponent<RigidBody3D>()) {
		EngineLogger::Error(aiObject->GetName() + " does not have either an AIComponent or a RigidBody3D. DynamicFlee has failed",
			"Dynamic.cpp", __LINE__);

		return false;
	}

	AIComponent* objectAIComp = aiObject->GetComponent<AIComponent>();
	DynamicSteeringOutput result = DynamicSteeringOutput();

	//gets direction
	result.linearAccel = aiObject->transform.GetPosition() - target->transform.GetPosition();

	result.linearAccel = VMath::normalize(result.linearAccel);
	result.linearAccel = objectAIComp->GetMaxAcceleration() * result.linearAccel;

	result.angularAccel = Vec3(0.0f);

	objectAIComp->SetDynamicSteering(result);

	return true;
}
#pragma endregion

DynamicArrive::DynamicArrive(GameObject* aiObject_, GameObject* target_, float arrivalRadius_, float slowRadius_, float timeToTarget_)	{
	aiObject = aiObject_;
	target = target_;
	arrivalRadius = arrivalRadius_;
	slowRadius = slowRadius_;
	if (timeToTarget_ > VERY_SMALL)	timeToTarget = timeToTarget_;
	else timeToTarget = 0.1f;
}

bool DynamicArrive::getSteering()	{
	//Checks if aiObject has the required components to function
	if (!aiObject->HasComponent<AIComponent>() || !aiObject->HasComponent<RigidBody3D>()) {
		EngineLogger::Error(aiObject->GetName() + " does not have either an AIComponent or a RigidBody3D. DynamicSeek has failed",
			"Dynamic.cpp", __LINE__);

		return false;
	}

	DynamicSteeringOutput result = DynamicSteeringOutput();

	//Get direction to target
	result.linearAccel = target->transform.GetPosition() - aiObject->transform.GetPosition();

	//check if in radius
	if (VMath::mag(result.linearAccel) < arrivalRadius) {
		//does not steer as it is in radius
		result.linearAccel = Vec3(0.0f);
		result.angularAccel = Vec3(0.0f);

		return false;
	}

	float speed;
	//if not in slow radius
	if (!VMath::mag(result.linearAccel) < slowRadius)	{
		//use max speed
		speed = aiObject->GetComponent<AIComponent>()->GetMaxSpeed();
	}
	//if it is
	else	{
		//uses slowed speed
		speed = aiObject->GetComponent<AIComponent>()->GetMaxSpeed() * VMath::mag(result.linearAccel) / slowRadius;
	}

	Vec3 desiredVelocity = VMath::normalize(result.linearAccel) * aiObject->GetComponent<AIComponent>()->GetMaxSpeed();

	//determine linearAcceleration we will be using
	result.linearAccel = desiredVelocity - aiObject->GetComponent<RigidBody3D>()->GetVelocity();
	result.linearAccel /= timeToTarget;

	//Cap the accel to max if needed
	if (VMath::mag(result.linearAccel) > aiObject->GetComponent<AIComponent>()->GetMaxAcceleration()) {
		result.linearAccel = VMath::normalize(result.linearAccel) * aiObject->GetComponent<AIComponent>()->GetMaxAcceleration();
	}
	result.angularAccel = Vec3(0.0f);

	//pass result to aiObject's AI
	aiObject->GetComponent<AIComponent>()->SetDynamicSteering(result);

	return true;
}

DynamicAlign::DynamicAlign(GameObject* aiObject_, GameObject* target_, float arrivalRadius_, float slowRadius_, float timeToTarget_)	{
	aiObject = aiObject_;
	target = target_;
	arrivalRadius = arrivalRadius_;
	slowRadius = slowRadius_;
	if (timeToTarget_ > VERY_SMALL)	timeToTarget = timeToTarget_;
	else timeToTarget = 0.1f;
}

bool DynamicAlign::getSteering()	{
	//checks if aiObject has the required components
	if (!aiObject->HasComponent<AIComponent>() || !aiObject->HasComponent<RigidBody3D>()) {
		EngineLogger::Error(aiObject->GetName() + " does not have either an AIComponent or a RigidBody3D. DynamicSeek has failed",
			"Dynamic.cpp", __LINE__);

		return false;
	}
	//checks if the target has the required components
	if (!target->HasComponent<RigidBody3D>()) {
		EngineLogger::Error(aiObject->GetName() + " does not have either an AIComponent or a RigidBody3D. DynamicSeek has failed",
			"Dynamic.cpp", __LINE__);

		return false;
	}

	//DynamicSteeringOutput result = DynamicSteeringOutput();

	//Vec3 rotation = target->transform.GetRotation() - aiObject->transform.GetRotation();

	////insert rotation correction (so that it turns in the fastest direction)

	////if at the target, do not rotate
	//if (VMath::mag(rotation) < arrivalRadius)	{
	//	return false;
	//}

	//float rotateSpeed;

	//if (VMath::mag(rotation) > slowRadius)	{
	//	rotateSpeed = aiObject->GetComponent<AIComponent>()->GetMaxRotateSpeed();
	//}
	//else {
	//	rotateSpeed = aiObject->GetComponent<AIComponent>()->GetMaxRotateSpeed() * VMath::mag(rotation) / slowRadius;
	//}

	//result.angularAccel = rotateSpeed - aiObject->transform.GetRotation();
	//result.angularAccel ..............

	return false;
}



DynamicObstacleAvoidance::DynamicObstacleAvoidance(GameObject* aiObject_, GameObject* target_, float avoidDistance_, float lookAhead_) : DynamicSeek(aiObject_, target_) {
	avoidDistance = avoidDistance_;
	lookAhead = lookAhead_;
}

bool DynamicObstacleAvoidance::getSteering()	{
	//this needs ray collision detection. The ray needs to auto check if a ray collides with anything (within a distance)

	return false;
}
