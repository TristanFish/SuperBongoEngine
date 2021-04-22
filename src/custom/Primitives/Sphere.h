#ifndef SPHERE_H
#define SPHERE_H


#include "components/ECS.h"
#include "components/Components.h"

class Sphere : public GameObject, public MeshRenderer, public RigidBody3D
{

public:
	Sphere();
	Sphere(const char* name, MATH::Vec3 position);
	~Sphere();


	// Inherited via GameObject
	void Update(const float deltaTime) override;
	void Render() const override;
	void HandleEvents(const SDL_Event& event) override;

	virtual void OnCollisionEnter(RigidBody3D& otherBody) override;


private:
};

#endif

