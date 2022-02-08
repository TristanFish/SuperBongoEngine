#ifndef BOX_H
#define BOX_H

#include "components/GameObject.h"

class Box : public GameObject
{

public:
	Box();
	Box(const std::string& name, MATH::Vec3 position);
	~Box();


	virtual void PostInit() override;
	virtual void OnCollisionEnter(Collider3D& otherBody) override;


	std::shared_ptr<GameObject> NewClone() const override { return std::make_shared<Box>(this->name, this->transform.GetPosition()); }

private:
};
#endif 
