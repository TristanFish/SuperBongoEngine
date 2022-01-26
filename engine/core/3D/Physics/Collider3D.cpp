#include "Collider3D.h"
#include "components/3D/RigidBody3D.h"

#include "math/Vector.h"

Collider3D::Collider3D() : B_IsMoveable(true), B_IsTrigger(false), V_Size(MATH::Vec3(1.0f,1.0f,1.0f)), Type(ColliderType::OBB), RB_Attached(nullptr), Model_Verticies(nullptr)
{

}

Collider3D::~Collider3D()
{
	RB_Attached = nullptr;
	Model_Verticies = nullptr;
}

void Collider3D::OnCollisionEnter(Collider3D& otherCollider)
{
	if (collisionEnterCallback != nullptr)
	{
		collisionEnterCallback(otherCollider);
	}
}

void Collider3D::AddCollisionFunction(std::function<void(Collider3D&)> func)
{
	collisionEnterCallback = func;
}

