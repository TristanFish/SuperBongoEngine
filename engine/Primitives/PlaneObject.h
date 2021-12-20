#ifndef PLANEOBJECT_H
#define PLANEOBJECT_H

#include "components/ECS.h"
#include "components/Components.h"

class PlaneObject : public GameObject
{

public:
	PlaneObject(const std::string& name, MATH::Vec3 position);
	~PlaneObject() override = default;
	
	void OnCollisionEnter(Collider3D& otherBody) override;

	PlaneObject* NewClone() const override { return new PlaneObject(this->name, this->transform.GetPosition()); };

};

#endif