#include "Transform.h"

#include "GameObject.h"

using namespace MATH;




Transform::Transform() : parent(nullptr), pos(Vec3()), scale(Vec3(1.0f)), rotation(MATH::Quaternion(1, Vec3(0.0, 0.0, 0.0)))
{
	
}

Transform::Transform(const MATH::Vec3& pos_) : parent(nullptr), pos(pos_), scale(MATH::Vec3(1.0f)), rotation(MATH::Quaternion(1, Vec3(0.0, 0.0, 0.0)))
{
	
}

void Transform::Update(const float deltaTime)
{
	UpdateMatricies();
}

Vec3 Transform::Forward() const
{
	return rotationMatrix * Vec3(0.0f, 0.0f, 1.0f);
}

Vec3 Transform::Right() const
{
	return rotationMatrix * Vec3(1.0f, 0.0f, 0.0f);
}

Vec3 Transform::Up() const
{
	return rotationMatrix * Vec3(0.0f, 1.0f, 0.0f);
}

void Transform::SetPos(const Vec3& pos)
{
	this->pos = pos;
	UpdateMatricies();
}

void Transform::SetRot(const Vec3& rot)
{
	rotation = Quaternion::EulerToQuat(rot);
	UpdateMatricies();

}

void Transform::SetRot(const Quaternion& rot)
{
	rotation = rot;
	UpdateMatricies();
}

void Transform::SetScale(const Vec3& scale)
{
	this->scale = scale;
	UpdateMatricies();
}

void Transform::UpdateMatricies()
{
	rotationMatrix = MMath::GetFromMat3(rotation.ConvertToMatrix());


	modelMatrix = MMath::translate(pos) * rotationMatrix * MMath::scale(scale);

	if (parent != nullptr)
	{
		modelMatrix = modelMatrix * parent->modelMatrix;
	}
}