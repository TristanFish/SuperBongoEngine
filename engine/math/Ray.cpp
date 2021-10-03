#include "Ray.h"

using namespace MATH;

Ray::Ray(Vec3 dir_, Vec3 origin_)
{
	direction = dir_;
	origin = origin_;
	distance = FLT_MAX;
}

Ray::Ray()
{
	direction = Vec3(0.0f);
	origin = Vec3(0.0f);
	distance = FLT_MAX;
}

Vec3 Ray::CurrentPosition(float t) const
{
	return Vec3(origin + direction * t);
}

std::string Ray::ToString() const
{
	std::string dir = "Dir: " + direction.ToString();
	std::string ori = "Ori: " + origin.ToString();
	
	return dir.append(ori);
}
