
#include "Ray.h"

Ray::Ray(const MATH::Vec3 dir_, const MATH::Vec3 origin_)
{
	direction = dir_;
	origin = origin_;
}

Ray::Ray()
{
	direction = MATH::Vec3(0.0f);
	origin = MATH::Vec3(0.0f);
}

Ray::~Ray()
{
}
