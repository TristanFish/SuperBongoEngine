
#include "Ray.h"

Ray::Ray(const MATH::Vec3 dir_, const MATH::Vec3 origin_)
{
	Direction = dir_;
	Origin = origin_;
}

Ray::Ray()
{
	Direction = MATH::Vec3(0.0f);
	Origin = MATH::Vec3(0.0f);
}

Ray::~Ray()
{
}
