#include "Physics.h"
#include "tiles/Tile.h"
#include "math/Vector.h"
#include "math/VMath.h"

using namespace MATH;
bool Physics::CircleCircleDetect(RigidBodyComponent& rb1, RigidBodyComponent& rb2)
{
	if (pow(rb1.collider.size + rb2.collider.size, 2) < VMath::dot(*rb1.pos, *rb2.pos))
	{
		std::cout << "Circle circle collision detected" << std::endl;
		return true;
	}
	return false;
}

//Found this code from https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection 
//check it out its got a great visual description.
bool Physics::CircleBoxDetect(RigidBodyComponent& circle, RigidBodyComponent& box)
{

	//Find the difference between both positions
	Vec3 differenceVector = *circle.pos - *box.pos; 
	
	//find the distance to the edge of the box and clamp the difference to find the closest contact point
	Vec3 clamped = MATH::VMath::clamp(differenceVector, MATH::Vec3(-box.collider.size / 2.0f),
															  MATH::Vec3(box.collider.size / 2.0f)); 
	Vec3 closestContactPoint = *box.pos + clamped;

	//distance from closest contact point to the center of the circle
	Vec3 distance = closestContactPoint - *circle.pos;

	
	if (VMath::mag(distance) < circle.collider.size / 2.0f)
	{
		return true;
	}
	return false;
}

bool Physics::BoxBoxDetect(RigidBodyComponent& rb1, RigidBodyComponent& rb2)
{
	float rb1Halfx = rb1.collider.size / 2.0f;
	float rb2Halfx = rb2.collider.size / 2.0f;
	float rb1Halfy = rb1Halfx;
	float rb2Halfy = rb2Halfx;
	if ((rb1.pos->x - rb1Halfx < rb2.pos->x + rb2Halfx) && //Check rb1 left edge with rb2 right edge
		(rb1.pos->x + rb1Halfx > rb2.pos->x - rb2Halfx) && //Check rb1 right edge with rb2 left edge
		(rb1.pos->y - rb1Halfy < rb2.pos->y + rb2Halfy) && //ChecK rb1 bottom edge with rb2 top edge
		(rb1.pos->y + rb1Halfy > rb2.pos->y - rb2Halfy))   //Check rb2 top edge with rb2 bottom edge
	{
		std::cout << "Box box collision detected" << std::endl;
		
		return true;
	}
	return false;
}

//returns the normal o
MATH::Vec3 Physics::CircleBoxClosestEdge(RigidBodyComponent& circle, RigidBodyComponent& box)
{
	//Find the difference between both positions
	Vec3 differenceVector = *circle.pos - *box.pos;

	//find the distance to the edge of the box and clamp the difference to find the closest contact point
	Vec3 clamped = VMath::clamp(differenceVector, Vec3(-box.collider.size / 2.0f), Vec3(box.collider.size / 2.0f));

	Vec3 closestContactPoint = *box.pos + clamped;

	//distance from closest contact point to the center of the circle
	Vec3 distance = closestContactPoint - *circle.pos;

	Vec3 normal = VMath::orthagonalize(clamped);
	normal.z = VMath::mag(distance);
	 
	return normal;
}

void Physics::CircleCircleResolve(RigidBodyComponent& rb1, RigidBodyComponent& rb2)
{
	if (rb1.collider.isMoveable)
	{
		//do some sort of collision resolution
	}

	if (rb2.collider.isMoveable)
	{
		//do some sort of collision resolution
	}

	rb1.OnCollisionEnter(rb2);
	rb2.OnCollisionEnter(rb1);
}

void Physics::CircleBoxResolve(RigidBodyComponent& circle, RigidBodyComponent& box)
{
	if (circle.collider.isMoveable && !box.collider.isTrigger)
	{
		Vec3 BoxNormal = CircleBoxClosestEdge(circle, box);
		
		Vec3 reflectVel = VMath::reflect(circle.vel, BoxNormal);

		circle.SetPosition(*circle.pos + (BoxNormal * 2.0f));
		std::cout << *circle.pos << std::endl;
		if (VMath::dot(reflectVel, BoxNormal) > -0.1f)
		{
			circle.vel += reflectVel * 0.5f;
		}
	}
	
	if (box.collider.isMoveable && !circle.collider.isTrigger)
	{

	}

	circle.OnCollisionEnter(box);
	box.OnCollisionEnter(circle);

}

void Physics::BoxBoxResolve(RigidBodyComponent& rb1, RigidBodyComponent& rb2)
{
	if (rb1.collider.isMoveable)
	{
		rb1.vel = -rb1.vel;
		
	}

	if (rb2.collider.isMoveable)
	{
		rb2.vel = -rb2.vel;
	}

	rb1.OnCollisionEnter(rb2);
	rb2.OnCollisionEnter(rb1);
}

bool Physics::DetectCollision(RigidBodyComponent& rb1, RigidBodyComponent& rb2)
{



	if (rb1.collider.colliderShape == Collider::shape::Circle && rb2.collider.colliderShape == Collider::shape::Circle)
	{
		if (CircleCircleDetect(rb1, rb2))
		{
			CircleCircleResolve(rb1, rb2);
			return true;
		}
		else
		{	
			return false;
		}
	} else
	if (rb1.collider.colliderShape == Collider::shape::Box && rb2.collider.colliderShape == Collider::shape::Circle)
	{
		if (CircleBoxDetect(rb2, rb1))
		{
			CircleBoxResolve(rb2, rb1);
			return true;
		}
		else 
		{
			return false;
		}
		
	} 
	else if (rb1.collider.colliderShape == Collider::shape::Circle && rb2.collider.colliderShape == Collider::shape::Box)
	{
		if (CircleBoxDetect(rb1, rb2))
		{
			CircleBoxResolve(rb1, rb2);
			return true;
		}
		else
		{
			return false;
		}
	} 
	else if (rb1.collider.colliderShape == Collider::shape::Box && rb2.collider.colliderShape == Collider::shape::Box)
	{
		if (BoxBoxDetect(rb1, rb2))
		{
			BoxBoxResolve(rb1, rb2);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		std::cout << "Collision shape detection went wrong" << std::endl;
	}
	return false;
}
