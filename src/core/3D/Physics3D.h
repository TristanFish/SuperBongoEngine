#ifndef PHYSICS3D_H
#define PHYSICS3D_H

#include "math/VMath.h"
#include "MMath.h"
#include "components/3D/RigidBody3D.h"

class Physics3D
{
private:

	static bool BoxBoxDetect(RigidBody3D& rb1, RigidBody3D& rb2);

	static void BoxBoxResolve(RigidBody3D& rb1, RigidBody3D& rb2);

	
public:
	
	bool DetectCollision(RigidBody3D& rb1, RigidBody3D& rb2);

};

#endif