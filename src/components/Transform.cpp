#include "Transform.h"

Transform::Transform() : pos(MATH::Vec3()), rotation(MATH::Vec3()), scale(MATH::Vec3(1.0f))
{
	rotationMatrix = MATH::MMath::calcRotationMatrix(rotation);
}

Transform::Transform(const MATH::Vec3& pos_) : pos(pos_), rotation(MATH::Vec3()), scale(MATH::Vec3(1.0f))
{
	rotationMatrix = MATH::MMath::calcRotationMatrix(rotation);
}

void Transform::Update(const float deltaTime)
{
	using namespace MATH;
	rotationMatrix = MMath::calcRotationMatrix(rotation);

	modelMatrix = MMath::translate(pos) * rotationMatrix * MMath::scale(scale);
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
	rotation = rot;
}

void Transform::SetScale(const MATH::Vec3& scale)
{
	this->scale = scale;
}
