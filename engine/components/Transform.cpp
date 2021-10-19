#include "Transform.h"

#include "GameObject.h"

using namespace MATH;


Transform::Transform() : parent(nullptr), pos(MATH::Vec3()), rotation(MATH::Quaternion(1, Vec3(0.0, 0.0, 0.0))), scale(MATH::Vec3(1.0f))
{
	

	//rotation = Quaternion(Vec3(0.0f, 0.0f, 1.0f), -90);
}

Transform::Transform(const MATH::Vec3& pos_) : parent(nullptr), pos(pos_), rotation(MATH::Quaternion(1, Vec3(0.0, 0.0, 0.0))), scale(MATH::Vec3(1.0f))
{
	
}

void Transform::Update(const float deltaTime)
{
	
	rotationMatrix =  MMath::GetFromMat3(rotation.ConvertToMatrix());


	modelMatrix =  MMath::translate(pos) * rotationMatrix * MMath::scale(scale);

	if(parent != nullptr)
	{
		modelMatrix = modelMatrix * parent->modelMatrix;
	}
}

MATH::Vec3 Transform::Forward() const
{
	return rotationMatrix * MATH::Vec3(0.0f, 0.0f, -1.0f);
}

MATH::Vec3 Transform::Right() const
{
	return rotationMatrix * MATH::Vec3(1.0f, 0.0f, 0.0f);
}

MATH::Vec3 Transform::Up() const
{
	return rotationMatrix * MATH::Vec3(0.0f, 1.0f, 0.0f);
}

void Transform::SetPos(const MATH::Vec3& pos)
{
	this->pos = pos;
}

void Transform::SetRot(const MATH::Vec3& rot)
{
	rotation = MATH::Quaternion::EulerToQuat(rot);
}

void Transform::SetRot(const Quaternion& rot)
{
	rotation = rot;
}

void Transform::SetScale(const MATH::Vec3& scale)
{
	this->scale = scale;
}
