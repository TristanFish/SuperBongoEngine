#ifndef PLAYER_H
#define PLAYER_H

#include "components/Components.h"
#include "scenes/Scene.h"

//Player inherits from gameobject, components are also added via inheritance
class Player : public GameObject, public MeshRenderer, public RigidBody3D
{
private:

public:

	Player();
	//Whenever making a gameobject make sure to give it a name and a position
	Player(const char* n, const MATH::Vec3& trans);
	virtual ~Player();



	//Inherited via GameObject
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;

	virtual void OnCollisionEnter(RigidBody3D& otherBody) override;
};

#endif