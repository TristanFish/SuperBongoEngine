#ifndef RAY_H
#define RAY_H
#include "VMath.h"
#include "Vector.h"
class Ray
{

	

public:
	MATH::Vec3 Direction;

	MATH::Vec3 Origin;

	Ray(const MATH::Vec3 dir_, const MATH::Vec3 origin_);

	Ray();

	~Ray();

private:


};
#endif
