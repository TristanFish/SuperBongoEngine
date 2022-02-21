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

	std::shared_ptr<GameObject> NewClone() const override { return std::make_shared<PlaneObject>(this->name, this->transform.GetPosition()); }

};

#endif