#ifndef COLLIDER_H
#define COLLIDER_H

//Not quite a component but similar enough to be in the same folder path
//Colliders define how rigidbodies will collide
struct Collider
{
public:

	enum class shape
	{
		Circle,
		Box
	};

	shape colliderShape;
	bool isMoveable, isTrigger;
	float size;

	Collider()
	{
		colliderShape = shape::Box;
		isMoveable = true;
		isTrigger = true;
		size = 1.0f;
	}
};

#endif