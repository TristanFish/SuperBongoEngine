#include "Physics3D.h"
#include "tiles/Tile.h"
#include "math/Vector.h"
#include "math/VMath.h"

using namespace MATH;

bool Physics3D::BoxBoxDetect(RigidBody3D& rb1, RigidBody3D& rb2)
{
	// Getting the min and max vertex of the object
	MATH::Vec3 p_min_1 = rb1.gameobject->getComponent<MeshRenderer>().p_min;
	MATH::Vec3 p_max_1 = rb1.gameobject->getComponent<MeshRenderer>().p_max;
	MATH::Vec3 p_min_2 = rb2.gameobject->getComponent<MeshRenderer>().p_min;
	MATH::Vec3 p_max_2 = rb2.gameobject->getComponent<MeshRenderer>().p_max;
	

	

	float rb1Halfx = ((abs(p_min_1.x) + abs(p_max_1.x)) / 2.0f);
	float rb2Halfx = ((abs(p_min_2.x) + abs(p_max_2.x)) / 2.0f);
	float rb1Halfy = ((abs(p_min_1.y) + abs(p_max_1.y)) / 2.0f);
	float rb2Halfy = ((abs(p_min_2.y) + abs(p_max_2.y)) / 2.0f);
	float rb1Halfz = ((abs(p_min_1.z) + abs(p_max_1.z)) / 2.0f);
	float rb2Halfz = ((abs(p_min_2.z) + abs(p_max_2.z)) / 2.0f);

	if ((rb1.pos->x - rb1Halfx < rb2.pos->x + rb2Halfx) && //Check rb1 left edge with rb2 right edge
		(rb1.pos->x + rb1Halfx > rb2.pos->x - rb2Halfx) && //Check rb1 right edge with rb2 left edge
		(rb1.pos->y - rb1Halfy < rb2.pos->y + rb2Halfy) && //ChecK rb1 bottom edge with rb2 top edge
		(rb1.pos->y + rb1Halfy > rb2.pos->y - rb2Halfy) && 
		(rb1.pos->z - rb1Halfz < rb2.pos->z + rb2Halfz) &&   //Check rb2 top edge with rb2 bottom edge
		(rb1.pos->z + rb1Halfz > rb2.pos->z - rb2Halfz))
	{	
		return true;
	}
	 
	return false;
}

bool Physics3D::SphereBoxDetect(RigidBody3D& rb1, RigidBody3D& rb2)
{
	RigidBody3D* sphere = nullptr;
	RigidBody3D* box = nullptr;

	if (rb1.GetColliderShape() == Collider3D::shape::AABB)
	{
		sphere = &rb2;
		box = &rb1;
	}
	else
	{
		sphere = &rb1;
		box = &rb2;
	}


	MATH::Vec3 p_min_1 = sphere->gameobject->getComponent<MeshRenderer>().p_min;
	MATH::Vec3 p_max_1 = sphere->gameobject->getComponent<MeshRenderer>().p_max;

	MATH::Vec3 p_min_2 = box->gameobject->getComponent<MeshRenderer>().p_min;
	MATH::Vec3 p_max_2 = box->gameobject->getComponent<MeshRenderer>().p_max;

	//Find the difference between both positions
	Vec3 differenceVector = *sphere->pos - *box->pos;

	//find the distance to the edge of the box and clamp the difference to find the closest contact point
	Vec3 clamped = MATH::VMath::clamp(differenceVector, p_min_2 /2,
		p_max_2 /2);
	Vec3 closestContactPoint = *box->pos + clamped;

	//distance from closest contact point to the center of the circle
	Vec3 distance = closestContactPoint - *sphere->pos;

	if (VMath::mag(distance) < (abs(p_max_1.x) + abs(p_min_1.x)) / 2 ||
		VMath::mag(distance) < (abs(p_max_1.y) + abs(p_min_1.y)) / 2 || 
		VMath::mag(distance) < (abs(p_max_1.z) + abs(p_min_1.z)) / 2)
	{
		return true;
	}
	
	return false;
}

MATH::Vec3 Physics3D::CircleBoxClosestEdge(RigidBody3D& sphere, RigidBody3D& box)
{
	//Find the difference between both positions
	Vec3 differenceVector = *sphere.pos - *box.pos;

	//find the distance to the edge of the box and clamp the difference to find the closest contact point
	Vec3 clamped = VMath::clamp(differenceVector, Vec3(-box.collider.size / 2.0f), Vec3(box.collider.size / 2.0f));

	Vec3 closestContactPoint = *box.pos + clamped;

	//distance from closest contact point to the center of the circle
	Vec3 distance = closestContactPoint - *sphere.pos;

	Vec3 normal = MMath::calcRotationMatrix(box.gameobject->transform.rotation) * VMath::orthagonalize(clamped);
	//normal.z = VMath::mag(distance);

	return normal;
}

void Physics3D::SphereBoxResolve(RigidBody3D& rb1, RigidBody3D& rb2)
{
	RigidBody3D* sphere = nullptr;
	RigidBody3D* box = nullptr;

	if (rb1.GetColliderShape() == Collider3D::shape::AABB)
	{
		box = &rb1;
		sphere = &rb2;
	}
	else
	{
		box = &rb2;
		sphere = &rb1;
	}

	if (sphere->collider.isMoveable && !box->collider.isTrigger)
	{
		Vec3 BoxNormal = CircleBoxClosestEdge(*sphere, *box);

		Vec3 reflectVel = VMath::reflect(sphere->vel, BoxNormal);

		sphere->SetPosition(*sphere->pos + (BoxNormal * 0.05f));
		if (VMath::dot(VMath::normalize(reflectVel), BoxNormal) > -0.1f)
		{
			sphere->vel += reflectVel * 0.5f;
		}
	}

	if (box->collider.isMoveable && !sphere->collider.isTrigger)
	{

	}
	box->OnCollisionEnter(*sphere);
	sphere->OnCollisionEnter(*box);
}

void Physics3D::BoxBoxResolve(RigidBody3D& rb1, RigidBody3D& rb2)
{
	if (rb1.collider.isMoveable)
	{
		rb1.vel.x = 0.0f;
		rb1.vel.y = 0.0f;
		rb1.vel.z = 0.0f;
		rb1.accel.x = 0.0f;
		rb1.accel.y = 0.0f;
		rb1.accel.z = 0.0f;
	}
	if (rb2.collider.isMoveable)
	{
		rb1.vel.x = 0.0f;
		rb1.vel.y = 0.0f;
		rb1.vel.z = 0.0f;
		rb1.accel.x = 0.0f;
		rb1.accel.y = 0.0f;
		rb1.accel.z = 0.0f;
	}

	//rb1.accel.print();
	rb1.OnCollisionEnter(rb2);
	rb2.OnCollisionEnter(rb1);
}

bool Physics3D::DetectCollision(RigidBody3D& rb1, RigidBody3D& rb2)
{
	switch (rb1.collider.colliderShape | rb2.collider.colliderShape)
	{
	case Collider3D::shape::AABB:
		if (BoxBoxDetect(rb1, rb2))
		{
			BoxBoxResolve(rb1, rb2);
			return true;
		}
		return false;
		break;
	case Collider3D::shape::Sphere | Collider3D::shape::AABB:
		if (SphereBoxDetect(rb1, rb2))
		{
			SphereBoxResolve(rb1, rb2);
			return true;
		}
		return false;
		break;
	default:
		std::cout << "Something went wrong in DetectCollision" << std::endl;
		return false;
		break;
	}
}

