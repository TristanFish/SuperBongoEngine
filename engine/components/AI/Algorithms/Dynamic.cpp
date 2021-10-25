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

void DynamicSteeringOutput::Update(const float deltaTime, GameObject* aiObject_)	{

	if (aiObject_->HasComponent<RigidBody3D>() && aiObject_->HasComponent<AIComponent>()) {

		RigidBody3D* aiBody = aiObject_->GetComponent<RigidBody3D>();

		aiBody->SetAngAccel(angularAccel);

		if (VMath::mag(linearAccel) > aiObject_->GetComponent<AIComponent>()->GetMaxAcceleration()) {
			linearAccel = VMath::normalize(linearAccel) * aiObject_->GetComponent<AIComponent>()->GetMaxAcceleration();
		}

		if(VMath::mag(aiBody->GetVelocity()) >= aiObject_->GetComponent<AIComponent>()->GetMaxSpeed())	{
			aiBody->SetVelocity(VMath::normalize(aiBody->GetVelocity()) * aiObject_->GetComponent<AIComponent>()->GetMaxSpeed());
		}
		
		aiBody->SetAccel(linearAccel);
		if(abs(VMath::mag(linearAccel)) <= VERY_SMALL)	{
			aiBody->SetVelocity(Vec3(0.0f));
		}
	}
	else {
		EngineLogger::Error(aiObject_->GetName() + " does not have either an AIComponent or a RigidBody3D. Cannot update steering",
			"Dynamic.cpp", __LINE__);
	}



}

#pragma region DynamicSeek
DynamicSeek::DynamicSeek(GameObject* aiObject_, const Transform& target_)	{
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

	DynamicSteeringOutput result = DynamicSteeringOutput();

	//gets direction
	result.linearAccel = target.GetPosition() - aiObject->transform.GetPosition();

	result.linearAccel = VMath::normalize(result.linearAccel);
	result.linearAccel = aiObject->GetComponent<AIComponent>()->GetMaxAcceleration() * result.linearAccel;

	aiObject->transform.SetRot(Quaternion::LookAt(aiObject->transform.GetPosition(), target.GetPosition(), aiObject->transform.Up()));
	
	result.angularAccel = Vec3(0.0f);

	aiObject->GetComponent<AIComponent>()->SetSteering(&result);

	return true;
}


#pragma endregion


#pragma region DynamicFlee
DynamicFlee::DynamicFlee(GameObject* aiObject_, const Transform& target_)	{
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

	DynamicSteeringOutput result = DynamicSteeringOutput();

	//gets direction
	result.linearAccel = aiObject->transform.GetPosition() - target.GetPosition();

	result.linearAccel = VMath::normalize(result.linearAccel);
	result.linearAccel = aiObject->GetComponent<AIComponent>()->GetMaxAcceleration() * result.linearAccel;

	result.angularAccel = Vec3(0.0f);

	aiObject->GetComponent<AIComponent>()->SetSteering(&result);

	return true;
}
#pragma endregion

DynamicArrive::DynamicArrive(GameObject* aiObject_, const Transform& target_, const float arrivalRadius_, const float slowRadius_, float timeToTarget_)	{
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
	result.linearAccel = target.GetPosition() - aiObject->transform.GetPosition();

	//check if in radius
	if (VMath::mag(result.linearAccel) < arrivalRadius) {
		//does not steer as it is in radius
		result.linearAccel = Vec3(0.0f);
		result.angularAccel = Vec3(0.0f);

		aiObject->GetComponent<AIComponent>()->SetSteering(&result);
		
		return false;
	}

	Vec3 desiredVelocity;
	//if not in slow radius
	if (!(VMath::mag(result.linearAccel) < slowRadius))	{
		//use max speed
		desiredVelocity = VMath::normalize(result.linearAccel) * aiObject->GetComponent<AIComponent>()->GetMaxSpeed();
	}
	//if it is
	else	{
		//uses slowed speed
		desiredVelocity = result.linearAccel * aiObject->GetComponent<AIComponent>()->GetMaxSpeed() / slowRadius;
	}

	//determine linearAcceleration we will be using
	result.linearAccel = desiredVelocity - aiObject->GetComponent<RigidBody3D>()->GetVelocity();
	result.linearAccel /= timeToTarget;

	//Cap the accel to max if needed
	if (VMath::mag(result.linearAccel) > aiObject->GetComponent<AIComponent>()->GetMaxAcceleration()) {
		result.linearAccel = VMath::normalize(result.linearAccel) * aiObject->GetComponent<AIComponent>()->GetMaxAcceleration();
	}
	result.angularAccel = Vec3(0.0f);

	//pass result to aiObject's AI
	aiObject->GetComponent<AIComponent>()->SetSteering(&result);

	return true;
}

DynamicAlign::DynamicAlign(GameObject* aiObject_, const Transform& target_, const float arrivalRadius_, const float slowRadius_, float timeToTarget_)	{
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



DynamicObstacleAvoidance::DynamicObstacleAvoidance(GameObject* aiObject_, const Transform& target_, const float avoidDistance_, const float lookAhead_) : DynamicSeek(aiObject_, target_) {
	avoidDistance = avoidDistance_;
	lookAhead = lookAhead_;
}

bool DynamicObstacleAvoidance::getSteering()	{
	//this needs ray collision detection. The ray needs to auto check if a ray collides with anything (within a distance)

	return false;
}
