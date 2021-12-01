#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "rendering/Camera.h"

struct Ray;
class BoundingBox;

class CollisionDetection {
public:
	CollisionDetection(const CollisionDetection&) = delete;
	CollisionDetection(CollisionDetection&&) = delete;
	CollisionDetection& operator=(const CollisionDetection&) = delete;
	CollisionDetection& operator=(CollisionDetection&&) = delete;

	CollisionDetection() = delete;
	~CollisionDetection();

	//Ray Collisions
	static Ray MousePosToWorldRay(MATH::Vec2 mouseCoords_, Camera* camera_);
	static bool RayOBBIntersection(Ray* ray_, BoundingBox* box_);

	static bool OBBIntersection(BoundingBox& Box1, BoundingBox& Box2);
	
};

#endif