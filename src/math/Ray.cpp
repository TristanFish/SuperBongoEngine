
#include "Ray.h"

using namespace MATH;

Ray::Ray(Vec3 dir_, Vec3 origin_)
{
	Direction = dir_;
	Origin = origin_;
	distance = FLT_MAX;
}

Ray::Ray()
{
	Direction = Vec3(0.0f);
	Origin = Vec3(0.0f);
	distance = FLT_MAX;
}

Vec3 Ray::CurrentPosition(float t) const
{
	return Vec3(Origin + Direction * t);
}

std::string Ray::ToString() const
{
	std::string dir = "Dir: " + Direction.ToString();
	std::string ori = "Ori: " + Origin.ToString();
	
	return dir.append(ori);
}
