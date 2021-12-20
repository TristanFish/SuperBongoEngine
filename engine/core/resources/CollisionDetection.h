#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "rendering/Camera.h"

struct Ray;
class BoundingBox;
class BoundingSphere;
class Collider3D;
class CollisionDetection {

private:



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


	static void ColliderIntersection(Collider3D* Collider1, Collider3D* Collider2);

	static bool OBBIntersection(BoundingBox& Box1, BoundingBox& Box2);
	static bool SphereIntersection(BoundingSphere& Sphere1, BoundingSphere& Sphere2);


	
};

#endif