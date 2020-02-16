#include "Transform.h"

Transform::Transform() : pos(MATH::Vec3()), rotation(MATH::Vec3()), scale(MATH::Vec3(1.0f))
{
}

Transform::Transform(const MATH::Vec3& pos_) : pos(pos_), rotation(MATH::Vec3()), scale(MATH::Vec3(1.0f))
{
}

void Transform::Update(const float deltaTime)
{
	modelMatrix = MATH::MMath::translate(pos) * MATH::MMath::scale(scale);
}

void Transform::setPos(const MATH::Vec3& pos_)
{

}