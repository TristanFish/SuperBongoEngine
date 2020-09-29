#ifndef PLAYER_H
#define PLAYER_H

#include "components/Components.h"
#include "scenes/Scene.h"

//Player inherits from gameobject, components are also added via inheritance
class Player : public GameObject, public SpriteComponent, public RigidBodyComponent, public AudioListenerComponent
{
private:

	float jetPower, health;

public:
	Scene* currentScene;

	Player();
	//Whenever making a gameobject make sure to give it a name and a position
	Player(const char* n, const MATH::Vec3& trans);
	virtual ~Player();



	//Inherited via GameObject
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;


	//Inherited via RigidBodyComponent
    virtual void OnCollisionEnter(RigidBodyComponent& otherBody) override;

	inline float getHealth() { return health; }
	inline void setHealth(float f) { health = f; }

	inline float getJetPower() { return jetPower; }
	inline void setJetPower(float f) { jetPower = f; }
};

#endif