#include "Physics3D.h"
#include "tiles/Tile.h"
#include "math/Vector.h"
#include "math/VMath.h"

using namespace MATH;

bool Physics3D::BoxBoxDetect(RigidBody3D& rb1, RigidBody3D& rb2)
{
	float rb1Halfx = rb1.collider.size.x / 2.0f;
	float rb2Halfx = rb2.collider.size.x / 2.0f;
	float rb1Halfy = rb1.collider.size.y / 2.0f;
	float rb2Halfy = rb2.collider.size.y / 2.0f;
	float rb1Halfz = rb1.collider.size.z / 2.0f;
	float rb2Halfz = rb2.collider.size.z / 2.0f;


	if ((rb1.pos->x - rb1Halfx < rb2.pos->x + rb2Halfx) && //Check rb1 left edge with rb2 right edge
		(rb1.pos->x + rb1Halfx > rb2.pos->x - rb2Halfx) && //Check rb1 right edge with rb2 left edge
		(rb1.pos->y - rb1Halfy < rb2.pos->y + rb2Halfy) && //ChecK rb1 bottom edge with rb2 top edge
		(rb1.pos->y + rb1Halfy > rb2.pos->y - rb2Halfy) && 
		(rb1.pos->z - rb1Halfz < rb2.pos->z + rb2Halfz) && 
		(rb1.pos->z + rb1Halfz > rb2.pos->z - rb2Halfz))   //Check rb2 top edge with rb2 bottom edge
	{
		std::cout << rb1.gameobject->name <<" Collided With " <<rb2.gameobject->name << std::endl;
		
		return true;
	}
	return false;
}

bool Physics3D::SphereBoxDetect(RigidBody3D& sphere, RigidBody3D& box)
{
	//Find the difference between both positions
	Vec3 differenceVector = *sphere.pos - *box.pos;

	//find the distance to the edge of the box and clamp the difference to find the closest contact point
	Vec3 clamped = MATH::VMath::clamp(differenceVector,-box.collider.size,
		box.collider.size / 2.0f);
	Vec3 closestContactPoint = *box.pos + clamped;

	//distance from closest contact point to the center of the circle
	Vec3 distance = closestContactPoint - *sphere.pos;

	distance.print();

	if (VMath::mag(distance) < 2.54 )
	{
		std::cout << "Sphere box collision detected" << std::endl;
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

	Vec3 normal = VMath::orthagonalize(clamped);
	//normal.z = VMath::mag(distance);

	return normal;
}

void Physics3D::SphereBoxResolve(RigidBody3D& sphere, RigidBody3D& box)
{
	if (sphere.collider.isMoveable && !box.collider.isTrigger)
	{
		Vec3 BoxNormal = CircleBoxClosestEdge(sphere, box);

		Vec3 reflectVel = VMath::reflect(sphere.vel, BoxNormal);

		sphere.SetPosition(*sphere.pos + (BoxNormal * 0.05f));
		std::cout << *sphere.pos << std::endl;
		if (VMath::dot(reflectVel, BoxNormal) > -0.1f)
		{
			sphere.vel += reflectVel * 0.5f;
		}
	}

	if (box.collider.isMoveable && !sphere.collider.isTrigger)
	{

	}
	box.OnCollisionEnter(sphere);
	sphere.OnCollisionEnter(box);
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

	rb1.accel.print();
	rb1.OnCollisionEnter(rb2);
	rb2.OnCollisionEnter(rb1);
}

bool Physics3D::DetectCollision(RigidBody3D& rb1, RigidBody3D& rb2)
{
	if (rb1.GetColliderShape() == Collider3D::shape::AABB && rb2.GetColliderShape() == Collider3D::shape::AABB)
	{
		if (BoxBoxDetect(rb1, rb2))
		{
			BoxBoxResolve(rb1, rb2);
			return true;
		}
		else { return false; }
	}

	if (rb1.GetColliderShape() == Collider3D::shape::Sphere && rb2.GetColliderShape() == Collider3D::shape::AABB)
	{
		if (SphereBoxDetect(rb1, rb2))
		{
			SphereBoxResolve(rb1, rb2);
			return true;
		}
		else { return false; }
	}
	if (rb1.GetColliderShape() == Collider3D::shape::AABB && rb2.GetColliderShape() == Collider3D::shape::Sphere)
	{
		if (SphereBoxDetect(rb2, rb1))
		{
			SphereBoxResolve(rb2, rb1);
			return true;
		}
		else { return false; }
	}
	return false;
}

