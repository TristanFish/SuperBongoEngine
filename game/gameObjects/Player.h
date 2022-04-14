#ifndef PLAYER_H
#define PLAYER_H


#include "components/GameObject.h"

//! Player Class
/*! This class controls how the user interact with the objects in the scene */
class Player : public GameObject
{
public:

	//!Alternate Player Constructor
	/*! Initialzes the variables in player */
	Player(const std::string& name, const MATH::Vec3& pos);

	//!Virtual Player Destructor
	/*! Destroys any of the players pointers/vectors */
	virtual ~Player() override = default;

	void PostInit() override;

	//!Virtual Update Function
	/*! Updates the players variables/transform */
	void Update(const float deltaTime) override;

	//!Virtual HandleEvents Function
	/*! Controls how the player will navigate the world */
	void HandleEvents(const SDL_Event& event) override;
	
	void OnCollisionEnter(Collider3D& otherCollider) override;


	std::shared_ptr<GameObject> NewClone() const override { return std::make_shared<Player>(this->name, this->transform.GetPosition()); }



private:

	bool HasLanded;
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
};

#endif