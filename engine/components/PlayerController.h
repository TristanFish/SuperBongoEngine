#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "GameObject.h"


class PlayerController : public GameObject
{
	public:

		//!Alternate Player Constructor
		/*! Initialzes the variables in player */
		PlayerController(const std::string & name, const MATH::Vec3 & pos);

		//!Virtual Player Destructor
		/*! Destroys any of the players pointers/vectors */
		virtual ~PlayerController() override = default;

		//!Virtual Update Function
		/*! Updates the players variables/transform */
		void Update(const float deltaTime) override;

		//!Virtual HandleEvents Function
		/*! Controls how the player will navigate the world */
		void HandleEvents(const SDL_Event & event) override;

		std::shared_ptr<GameObject> NewClone() const override { return std::make_shared<PlayerController>(this->name, this->transform.GetPosition()); }

	private:

		//!moveSpeed float
		/*! Holds how fast the player will fly while in scene mode */
		const float moveSpeed;
		//!turnSpeed float
		/*! Holds how fast the player will turn */
		const float turnSpeed;
	};



};

#endif