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


Quaternion SteeringOutput::calculateOrientation(GameObject* aiObject_, Vec3 direction_)	{
	//transform is in the RigidBody3D
	if (!aiObject_->HasComponent<RigidBody3D>()) {
		EngineLogger::Error(aiObject_->GetName() + " does not have a Transform. steeringFace has failed",
			"SteeringOutput.cpp", __LINE__);

		return aiObject_->transform.GetRotationQuat();
	}

	const Matrix4 lookAt = MMath::lookAt(aiObject_->transform.GetPosition(), aiObject_->transform.GetPosition() + direction_, aiObject_->transform.Up());

	//The line below also works (rotation from the origin) but is less mathematically clear imo
	//const Matrix4 lookAt = MMath::lookAt(Vec3(), direction_, aiObject_->transform.Up());
	
	return MMath::ConvertMatToQuat(lookAt);
	
	Vec3 direction = VMath::normalize(direction_);
	
	Quaternion aiRotation = aiObject_->transform.GetRotationQuat();

	Vec3 zAxis = Vec3(0.0f, 0.0f, 1.0f);
	
	Vec3 aiZAxis = aiRotation.Rotate(zAxis);

	//if its already facing
	if (aiZAxis == direction) {
		return aiRotation;
	}
	else if(aiZAxis == direction * -1.0f)	{
		return aiRotation * -1.0f;
	}

	//Otherwise face it
	Vec3 axis = VMath::cross(aiZAxis, direction);

	float axisMag = findMod(VMath::mag(axis), 2.0f);
	axisMag -= 1.0f;
	
	float angle = asin(abs(axisMag));
	axis = VMath::normalize(axis);

	
	float sinAngle = sin(angle / 2.0f);

	float radiansToDegrees = RADIANS_TO_DEGREES * cos(angle / 2);
	
	return Quaternion(sinAngle * axis.x, sinAngle * axis.y, sinAngle * axis.z, cos(angle / 2));
}




