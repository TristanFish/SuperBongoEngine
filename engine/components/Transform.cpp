#include "Transform.h"

#include "GameObject.h"

using namespace MATH;


Transform::Transform() : parent(nullptr), pos(Vec3()), scale(Vec3(1.0f)), rotation(Quaternion(1, Vec3(0.0, 0.0, 0.0))) {}

Transform::Transform(const Vec3& pos_) : parent(nullptr), pos(pos_), scale(Vec3(1.0f)), rotation(Quaternion(1, Vec3(0.0, 0.0, 0.0))) {}

void Transform::Update(const float deltaTime)
{
	rotationMatrix =  MMath::GetFromMat3(rotation.ConvertToMatrix());

	modelMatrix =  MMath::translate(pos) * rotationMatrix * MMath::scale(scale);

	if(parent != nullptr)
	{
		modelMatrix = parent->modelMatrix * modelMatrix;
	}
}

Vec3 Transform::Forward() const
{
	return rotation.Rotate(Vec3::Forward());
}

Vec3 Transform::Right() const
{
	return rotation.Rotate(Vec3::Right());
}

Vec3 Transform::Up() const
{
	return rotation.Rotate(Vec3::Up());
}

void Transform::SetPos(const Vec3& pos)
{
	this->pos = pos;
}

void Transform::SetRot(const Vec3& rot)
{
	rotation = Quaternion::EulerToQuat(rot);
}

void Transform::SetRot(const Quaternion& rot)
{
	rotation = rot;
}

void Transform::SetScale(const Vec3& scale)
{
	this->scale = scale;
}
