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
	bool isMoveable, isTrigger,isWiremesh;
	MATH::Vec3 size;

	Collider()
	{
		colliderShape = shape::Box;
		isMoveable = true;
		isTrigger = false;
		isWiremesh = false;
		size = MATH::Vec3(1.0f, 1.0f, 1.0f);
	}
};
#endif