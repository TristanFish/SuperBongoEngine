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
	Player(const char* n, const MATH::Vec3& pos);

	//!Virtual Player Destructor
	/*! Destroys any of the players pointers/vectors */
	virtual ~Player() override = default;

	//!Virtual Update Function
	/*! Updates the players variables/transform */
	void Update(const float deltaTime) override;

	//!Virtual HandleEvents Function
	/*! Controls how the player will navigate the world */
	void HandleEvents(const SDL_Event& event) override;

private:

	//!moveSpeed float
	/*! Holds how fast the player will fly while in scene mode */
	const float moveSpeed;

	//!turnSpeed float
	/*! Holds how fast the player will turn */
	const float turnSpeed;

	//!mouseDown boolean
	/*! Changes when the mouse down state gets changed */
	bool mouseDown;

	//!mouseStart Vector3
	/*! Used for detecting trackball player rotation */
	MATH::Vec3 mouseStart;

	//!mouseEnd Vector3
	/*! Used for detecting trackball player rotation  */
	MATH::Vec3 mouseEnd;

	//!lookMatrix Matrix4
	/*! Used for detecting trackball player rotation  */
	MATH::Matrix4 lookMatrix;


	//!OnMouseMove
	/*! Moves/rotates the player with the mouse*/
	void OnMouseMove(int x, int y);
};

#endif