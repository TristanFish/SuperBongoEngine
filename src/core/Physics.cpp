#include "Physics.h"

bool Physics::CircleCircleDetect(RigidBodyComponent& rb1, RigidBodyComponent& rb2)
{
	if (pow(rb1.collider.size + rb2.collider.size, 2) < pow((rb1.pos.x + rb2.pos.x), 2) + pow((rb1.pos.y + rb2.pos.y), 2))
	{
		return true;
	}
	return false;
}

//Found this code from https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection 
//check it out its got a great visual description. also not sure if it works yet
bool Physics::CircleBoxDetect(RigidBodyComponent& circle, RigidBodyComponent& box)
{
	//Find the difference between both positions
	MATH::Vec3 differenceVector = circle.pos - box.pos; 
	
	//find the distance to the edge of the box and clamp the difference to find the closest contact point
	MATH::Vec3 clamped = MATH::VMath::clamp(differenceVector, MATH::Vec3(-box.collider.size / 2.0f),
															  MATH::Vec3(box.collider.size / 2.0f)); 
	MATH::Vec3 closestContactPoint = box.pos + clamped;

	//distance from closest contact point to the center of the circle
	MATH::Vec3 distance = closestContactPoint - circle.pos;

	if (MATH::VMath::mag(distance) < circle.collider.size)
	{
		return true;
	}
	return false;
}

bool Physics::BoxBoxDetect(RigidBodyComponent& rb1, RigidBodyComponent& rb2)
{
	if ((rb1.pos.x < rb2.pos.x + rb2.collider.size) &&
		(rb1.pos.x + rb1.collider.size > rb2.pos.x) &&
		(rb1.pos.y < rb2.pos.y + rb2.collider.size) &&
		(rb1.pos.y + rb1.collider.size > rb2.pos.y))
	{
		return true;
	}
	return false;
}

void Physics::CircleCircleResolve(RigidBodyComponent& rb1, RigidBodyComponent& rb2)
{
}

void Physics::CircleBoxResolve(RigidBodyComponent& rb1, RigidBodyComponent& rb2)
{
}

void Physics::BoxBoxResolve(RigidBodyComponent& rb1, RigidBodyComponent& rb2)
{
}

bool Physics::DetectCollision(RigidBodyComponent& rb1, RigidBodyComponent& rb2)
{
	if (rb1.collider.colliderShape == Collider::Circle && rb2.collider.colliderShape == Collider::Circle)
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
	if (rb1.collider.colliderShape == Collider::Box && rb2.collider.colliderShape == Collider::Circle)
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
	else if (rb1.collider.colliderShape == Collider::Circle && rb2.collider.colliderShape == Collider::Box)
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
	else if (rb1.collider.colliderShape == Collider::Box && rb2.collider.colliderShape == Collider::Box)
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

void Physics::ResolveCollision(RigidBodyComponent& rb1, RigidBodyComponent& rb2)
{
}
