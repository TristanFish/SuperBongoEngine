#ifndef RAY_H
#define RAY_H
#include "VMath.h"
#include "Vector.h"
#include "core/resources/CollisionDetection.h"
#include <vector>


struct Ray
{
public:
	MATH::Vec3 direction;
	MATH::Vec3 origin;
	MATH::Vec3 end;
	float distance;

	Ray();

	Ray(MATH::Vec3 dir_, MATH::Vec3 origin_);

	//Copy function
	inline Ray& operator =(const Ray& ray_) {
		origin = ray_.origin;
		direction = ray_.direction;
		end = ray_.end;
		distance = 0.0f;

		return *this;
	}

	~Ray() = default;

	MATH::Vec3 CurrentPosition(float t) const;

	bool IntersectsLineSegment(const MATH::Vec2& p1, const MATH::Vec2& p2) const;

	inline bool isColliding(BoundingBox* box_) {
		distance = -1.0f;
		return CollisionDetection::RayOBBIntersection(this, box_);
	}
	
	std::string ToString() const;

};
#endif
