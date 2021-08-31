#ifndef BOX_H
#define BOX_H

#include "components/GameObject.h"

class Box : public GameObject
{

public:
	Box();
	Box(std::string name, MATH::Vec3 position);
	~Box();


	virtual void OnCollisionEnter(RigidBody3D& otherBody) override;

	Box* GetClone() const override { return new Box(*this); };


private:
};
#endif 
