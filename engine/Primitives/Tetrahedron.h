
#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H


#include "components/GameObject.h"

class Tetrahedron : public GameObject
{

public:
	Tetrahedron();
	Tetrahedron(const std::string& name, MATH::Vec3 position);
	~Tetrahedron();


	// Inherited via GameObject
	virtual void OnCollisionEnter(RigidBody3D& otherBody) override;

	Tetrahedron* NewClone() const override { return new Tetrahedron(this->name, this->transform.GetPosition()); }


private:
};

#endif