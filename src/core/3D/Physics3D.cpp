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
		std::cout << "Box box collision detected" << std::endl;

		return true;
	}
	return false;
}

void Physics3D::BoxBoxResolve(RigidBody3D& rb1, RigidBody3D& rb2)
{
	if (rb1.collider.isMoveable)
	{
		// Resolving Action
	}
	if (rb2.collider.isMoveable)
	{
		// Resolving Action
	}
}

bool Physics3D::DetectCollision(RigidBody3D& rb1, RigidBody3D& rb2)
{
	if (rb1.GetColliderShape() == Collider::shape::Box && rb2.GetColliderShape() == Collider::shape::Box)
	{
		if (BoxBoxDetect(rb1, rb2))
		{
			BoxBoxResolve(rb1, rb2);
			return true;
		}
		else { return false; }
	}

	return false;
}

