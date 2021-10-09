#include "Dynamic.h"

#include "components/GameObject.h"
#include "components/AI/AIComponent.h"

using namespace Dynamic;

DynamicSteeringOutput::DynamicSteeringOutput()	{
	linearVel = Vec3(0.0f);
	angularVel = Vec3(0.0f);
}

DynamicSteeringOutput::DynamicSteeringOutput(Vec3 linearVel_, Vec3 angularVel_)	{
	linearVel = linearVel_;
	angularVel = angularVel_;
}

#pragma region DynamicSeek
DynamicSeek::DynamicSeek(GameObject* aiObject_, GameObject* target_)	{
	aiObject = aiObject_;
	target = target_;
}

bool DynamicSeek::getSteering()	{
	//Checks if aiObject has the required components to function
	if (!aiObject->HasComponent<AIComponent>() || !aiObject->HasComponent<RigidBody3D>()) {
		EngineLogger::Error(aiObject->GetName() + " does not have either an AIComponent or a RigidBody3D. DynamicSteering has failed",
			"Dynamic.cpp", __LINE__);

		return false;
	}

	AIComponent* objectAIComp = aiObject->GetComponent<AIComponent>();
	DynamicSteeringOutput result = DynamicSteeringOutput();

	//gets direction
	result.linearVel = target->transform.GetPosition() - aiObject->transform.GetPosition();

	result.linearVel = VMath::normalize(result.linearVel);
	result.linearVel = objectAIComp->GetMaxAcceleration() * result.linearVel;

	result.angularVel = Vec3(0.0f);

	objectAIComp->SetDynamicSteering(result);

	return true;
}


#pragma endregion