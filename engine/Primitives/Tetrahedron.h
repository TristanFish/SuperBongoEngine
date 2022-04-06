
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
	virtual void OnCollisionEnter(Collider3D& otherBody) override;
	virtual void PostInit() override;


	std::shared_ptr<GameObject> NewClone() const override { return std::make_shared<Tetrahedron>(this->name, this->transform.GetPosition()); }


private:
};

#endif