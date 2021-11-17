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
	end = Vec3(0.0f);
	distance = FLT_MAX;
}

Vec3 Ray::CurrentPosition(float t) const
{
	return Vec3(origin + direction * t);
}

bool Ray::IntersectsLineSegment(const Vec2& p1, const Vec2& p2) const
{
	
	Vec2 r = (p2 - p1);

	Vec2 s = Vec2(end.x , end.y) - Vec2(origin.x, origin.y);



	float d = r.x * s.y - r.y * s.x;

	float u = ((origin.x - p1.x) * r.y - (origin.y - p1.y) * r.x) / d;
	float t = ((origin.x - p1.x) * s.y - (origin.y - p1.y) * s.x) / d;


	

	if ((u > 0.0f && u <= 0.98f) && (t > 0.0f && t <= 0.98f))
		return true;


	return false;
}

std::string Ray::ToString() const
{
	std::string dir = "Dir: " + direction.ToString();
	std::string ori = "Ori: " + origin.ToString();
	
	return dir.append(ori);
}
