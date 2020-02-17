#include "Player.h"

Player::Player(const char* n, const MATH::Vec3& pos)
{
	name = n;
	transform = Transform(pos);

	//Always initialize the components that you've inherited with your current gameobject
	//this allows the components to access the transform of of your gameobject
	RigidBodyComponent::Init(this);
	SpriteComponent::Init(this);
}

Player::~Player()
{
	
}

void Player::Update(const float deltaTime)
{
	//Always update your inherited components and transform
	transform.Update(deltaTime);
	RigidBodyComponent::Update(deltaTime);
	SpriteComponent::Update(deltaTime);
}

void Player::Render() const
{
	//Render your inherited components
	RigidBodyComponent::Render();
	SpriteComponent::Render();
}

void Player::HandleEvents(const SDL_Event& event)
{
	if (event.type == SDL_EventType::SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
	{
		std::cout << "Spacebar pressed" << std::endl;
	}

	RigidBodyComponent::HandleEvents(event);
	SpriteComponent::HandleEvents(event);
}
