#ifndef SPHERE_H
#define SPHERE_H


#include "components/GameObject.h"

class Sphere : public GameObject
{

public:
	Sphere();
	Sphere(const char* name, MATH::Vec3 position);
	~Sphere();


	// Inherited via GameObject
	virtual void OnCollisionEnter(RigidBody3D& otherBody) override;

	Sphere* GetClone() const override { return new Sphere(*this); };


private:
};

#endif

