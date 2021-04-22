#ifndef PLANE_H
#define PLANE_H

#include "components/ECS.h"
#include "components/Components.h"

class Plane : public GameObject
{

public:
	Plane(const char* name, MATH::Vec3 position);
	~Plane() override = default;
	
	void OnCollisionEnter(RigidBody3D& otherBody) override;
};

#endif