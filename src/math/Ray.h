#ifndef RAY_H
#define RAY_H
#include "VMath.h"
#include "Vector.h"
class Ray
{

	

public:
	MATH::Vec3 Direction;
	MATH::Vec3 Origin;
	float distance;

	Ray();

	Ray(MATH::Vec3 dir_, MATH::Vec3 origin_);

	~Ray() = default;

	MATH::Vec3 CurrentPosition(float t) const;

	bool IsColliding()
	{
		return true;
	}

	std::string ToString() const;

private:


};
#endif
