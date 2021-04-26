#ifndef PHYSICS3D_H
#define PHYSICS3D_H

#include "math/VMath.h"
#include "MMath.h"
#include "Ray.h"
#include "components/3D/RigidBody3D.h"

class Physics3D
{
private:

	static bool BoxBoxDetect(RigidBody3D& rb1, RigidBody3D& rb2);
	static bool SphereBoxDetect(RigidBody3D& sphere, RigidBody3D& box);
	static bool BoxPlaneDetect(RigidBody3D& box, RigidBody3D& plane);
	static bool SpherePlaneDetect(RigidBody3D& sphere, RigidBody3D& plane);
	static bool RayOBBDetect(const Ray& ray, const OrientedBoundingBox& obb);
	static MATH::Vec3 CircleBoxClosestEdge(RigidBody3D& circle, RigidBody3D& box);


	static void SphereBoxResolve(RigidBody3D& sphere, RigidBody3D& box);
	static void BoxBoxResolve(RigidBody3D& rb1, RigidBody3D& rb2);
	static void BoxPlaneResolve(RigidBody3D& box, RigidBody3D& plane);
	static void SpherePlaneResolve(RigidBody3D& sphere, RigidBody3D& plane);

	
public:
	static bool DetectCollision(RigidBody3D& rb1, RigidBody3D& rb2);

};
#endif