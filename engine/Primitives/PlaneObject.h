#ifndef PLANEOBJECT_H
#define PLANEOBJECT_H

#include "components/ECS.h"
#include "components/Components.h"

class PlaneObject : public GameObject
{

public:
	PlaneObject(std::string name, MATH::Vec3 position);
	~PlaneObject() override = default;
	
	void OnCollisionEnter(RigidBody3D& otherBody) override;

	PlaneObject* GetClone() const override { return new PlaneObject(this->name, this->transform.pos); };

};

#endif