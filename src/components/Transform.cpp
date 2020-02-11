#include "Transform.h"

Tranform::Tranform() : pos(MATH::Vec3()), rotation(MATH::Vec3()), scale(MATH::Vec3(1.0f))
{
}

Tranform::Tranform(const MATH::Vec3& pos_) : pos(pos_), rotation(MATH::Vec3()), scale(MATH::Vec3(1.0f))
{
}

void Tranform::Update(const float deltaTime)
{
	modelMatrix = MATH::MMath::translate(pos) * MATH::MMath::scale(scale);
}

void Tranform::setPos(const MATH::Vec3& pos_)
{

}