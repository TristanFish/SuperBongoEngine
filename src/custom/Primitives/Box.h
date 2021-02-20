#ifndef BOX_H
#define BOX_H

#include "components/ECS.h"
#include "components/Components.h"

class Box : public GameObject, public MeshRenderer, public RigidBody3D
{

public:
	Box();
	Box(const char* name, MATH::Vec3 position);
	~Box();


	// Inherited via GameObject
	void Update(const float deltaTime) override;
	void Render() const override;
	void HandleEvents(const SDL_Event& event) override;


	 Type getType() const override;

	virtual void OnCollisionEnter(RigidBody3D& otherBody) override;


private:
};
#endif 
