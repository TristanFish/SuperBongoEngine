#include "Plane.h"

#include "VMath.h"

using namespace MATH;

Plane::Plane() : Vec3(0.0f, 1.0f, 0.0f)
{
	d = 0;
}

Plane::Plane(float x, float y, float z, float d) : Vec3(x, y, z)
{
	this->d = d;
}

Plane::Plane(const Vec3& v, float d) : Vec3(v)
{
	this->d = d;
}

Plane::Plane(const Vec3& v1, const Vec3& v2, const Vec3& v3)
{
	
}

float Plane::DistanceFromPoint(const Vec3& v) const
{
	return (x * v.x + y * v.y + z * v.z + d);
}

Vec3 Plane::IntersectionPointWithRay(const Ray& r) const
{
	//float planePoint = -(Vec3(x,y,z))
	return Vec3();
}

void Plane::Normalize()
{
	float mag = VMath::mag(*this);
	x /= mag;
	y /= mag;
	z /= mag;

	d /= mag;
	
}
