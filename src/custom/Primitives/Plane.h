#ifndef PLANE_H
#define PLANE_H

#include "components/ECS.h"
#include "components/Components.h"

class Plane : public GameObject, public MeshRenderer, public RigidBody3D
{

public:
	Plane();
	Plane(const char* name, MATH::Vec3 position);
	~Plane();


	// Inherited via GameObject
	void Update(const float deltaTime) override;
	void Render() const override;
	void HandleEvents(const SDL_Event& event) override;

	Type getType() const override;
	virtual void OnCollisionEnter(RigidBody3D& otherBody) override;


private:
};

#endif

