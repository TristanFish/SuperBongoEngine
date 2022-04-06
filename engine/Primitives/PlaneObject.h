#ifndef PLANEOBJECT_H
#define PLANEOBJECT_H



#include "components/GameObject.h"

class PlaneObject : public GameObject
{

public:
	PlaneObject(const std::string& name, MATH::Vec3 position);
	~PlaneObject() override = default;
	
	void OnCollisionEnter(Collider3D& otherBody) override;

	virtual void PostInit() override;


	std::shared_ptr<GameObject> NewClone() const override { return std::make_shared<PlaneObject>(this->name, this->transform.GetPosition()); }

};

#endif