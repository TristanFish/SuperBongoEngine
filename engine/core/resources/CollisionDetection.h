#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "Rendering/CameraManager.h"
#include "core/3D/Physics/Collider3D.h"

struct Ray;
class BoundingBox;
class BoundingSphere;
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
	static Ray MousePosToWorldRay(MATH::Vec2 mouseCoords_);
	static bool RayOBBIntersection(Ray* ray_, BoundingBox* box_);


	static bool ColliderIntersection(Collider3D* Collider1, Collider3D* Collider2);

	static bool GJKIntersection(Collider3D& Box1, Collider3D& Box2);
	static bool SphereIntersection(BoundingSphere& Sphere1, BoundingSphere& Sphere2);


	
};

#endif