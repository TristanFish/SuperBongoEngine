#include "Physics3D.h"
#include "tiles/Tile.h"
#include "math/Vector.h"
#include "math/VMath.h"

using namespace MATH;

bool Physics3D::BoxBoxDetect(RigidBody3D& rb1, RigidBody3D& rb2)
{
	// TO-DO(Tristan) OPTIMISE THE SHIT OUTTA THIS 
	float rb1Halfx = ((abs(rb1.collider.minVertices.x) + abs(rb1.collider.maxVertices.x)) / 2.0f);
	float rb2Halfx = ((abs(rb2.collider.minVertices.x) + abs(rb2.collider.maxVertices.x)) / 2.0f);
	float rb1Halfy = ((abs(rb1.collider.minVertices.y) + abs(rb1.collider.maxVertices.y)) / 2.0f);
	float rb2Halfy = ((abs(rb2.collider.minVertices.y) + abs(rb2.collider.maxVertices.y)) / 2.0f);
	float rb1Halfz = ((abs(rb1.collider.minVertices.z) + abs(rb1.collider.maxVertices.z)) / 2.0f);
	float rb2Halfz = ((abs(rb2.collider.minVertices.z) + abs(rb2.collider.maxVertices.z)) / 2.0f);

	if ((rb1.pos->x - rb1Halfx < rb2.pos->x + rb2Halfx) && //Check rb1 left edge with rb2 right edge
		(rb1.pos->x + rb1Halfx > rb2.pos->x - rb2Halfx) && //Check rb1 right edge with rb2 left edge
		(rb1.pos->y - rb1Halfy < rb2.pos->y + rb2Halfy) && //ChecK rb1 bottom edge with rb2 top edge
		(rb1.pos->y + rb1Halfy > rb2.pos->y - rb2Halfy) && 
		(rb1.pos->z - rb1Halfz < rb2.pos->z + rb2Halfz) &&   //Check rb2 top edge with rb2 bottom edge
		(rb1.pos->z + rb1Halfz > rb2.pos->z - rb2Halfz))
	{	
		std::cout << "Box box collision detected" << std::endl;
		return true;
	}
	 
	return false;
}

bool Physics3D::SphereBoxDetect(RigidBody3D& sphere, RigidBody3D& box)
{
	
	//Find the difference between both positions
	Vec3 differenceVector = *sphere.pos - *box.pos;

	//find the distance to the edge of the box and clamp the difference to find the closest contact point
	Vec3 clamped = MATH::VMath::clamp(differenceVector, box.collider.minVertices /2,
		box.collider.maxVertices /2);
	Vec3 closestContactPoint = *box.pos + clamped;

	//distance from closest contact point to the center of the circle
	Vec3 distance = closestContactPoint - *sphere.pos;

	if (VMath::mag(distance) < (abs(sphere.collider.maxVertices.x) + abs(sphere.collider.minVertices.x)) / 2 ||
		VMath::mag(distance) < (abs(sphere.collider.maxVertices.y) + abs(sphere.collider.minVertices.y)) / 2 ||
		VMath::mag(distance) < (abs(sphere.collider.maxVertices.z) + abs(sphere.collider.minVertices.z)) / 2)
	{
		std::cout << "Sphere box collision detected" << std::endl;
		return true;
	}
	
	return false;
}

bool Physics3D::BoxPlaneDetect(RigidBody3D& box, RigidBody3D& plane)
{
	
	Vec3 extents = (box.collider.maxVertices - box.collider.minVertices) / 2.0f;
	extents += box.GetPosition() * -1;
	Vec3 centre = box.collider.minVertices + extents;

	Vec3 pNormal =  plane.gameobject->transform.Up() / 2;

	float r = (pNormal.x * extents.x) + (pNormal.y * extents.y) + (pNormal.z * extents.z);

	
	float dot = VMath::dot(pNormal, centre) - VMath::distance(box.GetPosition(), plane.GetPosition());
	

	std::cout << dot << std::endl;

	std::cout << r << std::endl;

	if(dot >= r)
	{
		return false;
	}
	if (dot <= -r)
	{
		return false;
	}
	std::cout << "Box Plane Collision Detected" << 1 << std::endl;
	return true;
}

bool Physics3D::SpherePlaneDetect(RigidBody3D& sphere, RigidBody3D& plane)
{

	//Vec3 closestPoint 
	return false;
}

MATH::Vec3 Physics3D::CircleBoxClosestEdge(RigidBody3D& sphere, RigidBody3D& box)
{
	//Find the difference between both positions
	Vec3 differenceVector = *sphere.pos - *box.pos;

	//find the distance to the edge of the box and clamp the difference to find the closest contact point
	Vec3 clamped = VMath::clamp(differenceVector, box.collider.minVertices / 2, box.collider.maxVertices / 2);

	Vec3 closestContactPoint = *box.pos + clamped;

	//distance from closest contact point to the center of the circle
	Vec3 distance = closestContactPoint - *sphere.pos;

	Vec3 normal = VMath::orthagonalize(clamped);
	normal.z = VMath::mag(distance);

	return normal;
}

void Physics3D::SphereBoxResolve(RigidBody3D& sphere, RigidBody3D& box)
{
	if (sphere.collider.isMoveable && !box.collider.isTrigger)
	{
		Vec3 BoxNormal = CircleBoxClosestEdge(sphere, box);

		Vec3 reflectVel = VMath::reflect(sphere.vel, BoxNormal);

		sphere.SetPosition(*sphere.pos + (BoxNormal * 0.05f));
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
		rb2.vel.x = 0.0f;
		rb2.vel.y = 0.0f;
		rb2.vel.z = 0.0f;
		rb2.accel.x = 0.0f;
		rb2.accel.y = 0.0f;
		rb2.accel.z = 0.0f;
	}

	//rb1.accel.print();
	rb1.OnCollisionEnter(rb2);
	rb2.OnCollisionEnter(rb1);
}

void Physics3D::BoxPlaneResolve(RigidBody3D& box, RigidBody3D& plane)
{
	std::cout << "Box Plane Collision Detected" << std::endl;
	if (box.collider.isMoveable)
	{
		box.vel.x = 0.0f;
		box.vel.y = 0.0f;
		box.vel.z = 0.0f;
		box.accel.x = 0.0f;
		box.accel.y = 0.0f;
		box.accel.z = 0.0f;
	}
	if (plane.collider.isMoveable)
	{
		plane.vel.x = 0.0f;
		plane.vel.y = 0.0f;
		plane.vel.z = 0.0f;
		plane.accel.x = 0.0f;
		plane.accel.y = 0.0f;
		plane.accel.z = 0.0f;
	}

	box.OnCollisionEnter(box);
	plane.OnCollisionEnter(box);
}

void Physics3D::SpherePlaneResolve(RigidBody3D& sphere, RigidBody3D& plane)
{
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

	if (rb1.GetColliderShape() == Collider3D::shape::AABB && rb2.GetColliderShape() == Collider3D::shape::Plane)
	{
		if (BoxPlaneDetect(rb1, rb2))
		{
			BoxPlaneResolve(rb1, rb2);
			return true;
		}
		else { return false; }
	}
	if (rb1.GetColliderShape() == Collider3D::shape::Plane && rb2.GetColliderShape() == Collider3D::shape::AABB)
	{
		if (BoxPlaneDetect(rb2, rb1))
		{
			BoxPlaneResolve(rb2, rb1);
			return true;
		}
		else { return false; }
	}
	return false;
}

