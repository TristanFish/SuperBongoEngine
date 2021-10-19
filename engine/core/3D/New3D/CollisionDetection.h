#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "rendering/Camera.h"

struct Ray;
struct BoundingBox;

class CollisionDetection {
public:
	CollisionDetection(const CollisionDetection&) = delete;
	CollisionDetection(CollisionDetection&&) = delete;
	CollisionDetection& operator=(const CollisionDetection&) = delete;
	CollisionDetection& operator=(CollisionDetection&&) = delete;

	CollisionDetection() = delete;
	~CollisionDetection();

	//Ray Collisions
	static Ray MousePosToWorldRay(MATH::Vec2 mouseCoords_, float screenWidth_, float screenHeight_, Camera* camera_);
	static bool RayObbIntersection(Ray* ray_, BoundingBox* box_);

	//
};

#endif