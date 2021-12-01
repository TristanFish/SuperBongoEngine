#ifndef PLAYER_H
#define PLAYER_H

#include "components/Components.h"

//! Player Class
/*! This class controls how the user interact with the objects in the scene */
class Player : public GameObject
{
public:

	//!Alternate Player Constructor
	/*! Initialzes the variables in player */
	Player(std::string name, const MATH::Vec3& pos);

	//!Virtual Player Destructor
	/*! Destroys any of the players pointers/vectors */
	virtual ~Player() override = default;

	//!Virtual Update Function
	/*! Updates the players variables/transform */
	void Update(const float deltaTime) override;

	//!Virtual HandleEvents Function
	/*! Controls how the player will navigate the world */
	void HandleEvents(const SDL_Event& event) override;

	Player* NewClone() const override { return new Player(this->name, this->transform.GetPosition()); };

private:

	//!moveSpeed float
	/*! Holds how fast the player will fly while in scene mode */
	const float moveSpeed;

	//!turnSpeed float
	/*! Holds how fast the player will turn */
	const float turnSpeed;
};

#endif