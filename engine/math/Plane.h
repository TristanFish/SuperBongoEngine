#ifndef PLANE_H
#define PLANE_H
#include "Ray.h"
#include "Vector.h"

namespace MATH
{
	struct Plane : Vec3
	{
		float d;
		
		Plane();
		Plane(float x, float y, float z, float d);
		Plane(const Vec3& v, float d);
		Plane(const Vec3& v1, const Vec3& v2, const Vec3& v3);

		float DistanceFromPoint(const Vec3& v) const;
		Vec3 IntersectionPointWithRay(const Ray& r) const;
		void Normalize();
	};
}

#endif