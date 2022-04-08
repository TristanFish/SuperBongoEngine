#include "SteeringOutput.h"
#include "components/GameObject.h" //Transform.h is included in GameObject
#include "components/AI/AIComponent.h"
#include "VMath.h"


SteeringOutput::SteeringOutput()	{

}

double findMod(double a, double b)
{
	double mod;
	// Handling negative values
	if (a < 0)
		mod = -a;
	else
		mod = a;
	if (b < 0)
		b = -b;

	// Finding mod by repeated subtraction

	while (mod >= b)
		mod = mod - b;

	// Sign of result typically depends
	// on sign of a.
	if (a < 0)
		return -mod;

	return mod;
}


Quaternion SteeringOutput::calculateOrientation(std::shared_ptr<GameObject> aiObject_, Vec3 direction_)	{
	//transform is in the RigidBody3D
	if (!aiObject_->HasComponent<RigidBody3D>()) {
		EngineLogger::Error(aiObject_->GetName() + " does not have a Transform. steeringFace has failed",
			"SteeringOutput.cpp", __LINE__);

		return aiObject_->transform.GetRotationQuat();
	}

	//const Matrix4 lookAt = MMath::lookAt(Vec3(), direction_, aiObject_->transform.Up());
	
	//return MMath::ConvertMatToQuat(lookAt);

	return Quaternion::LookAt(aiObject_->transform.GetRotationQuat(), direction_, Vec3::Forward());
}




