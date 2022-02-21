#ifndef SPHERE_H
#define SPHERE_H


#include "components/GameObject.h"

class Sphere : public GameObject
{

public:
	Sphere();
	Sphere(const std::string& name, MATH::Vec3 position);
	~Sphere();


	// Inherited via GameObject
	virtual void OnCollisionEnter(Collider3D& otherBody) override;


	std::shared_ptr<GameObject> NewClone() const override { return std::make_shared<Sphere>(this->name, this->transform.GetPosition()); }

private:
};

#endif

