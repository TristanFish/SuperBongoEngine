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
	virtual void OnCollisionEnter(RigidBody3D& otherBody) override;

	Sphere* NewClone() const override { return new Sphere(this->name, this->transform.pos); };


private:
};

#endif

