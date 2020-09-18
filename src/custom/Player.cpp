#include "Player.h"
#include "core/Timer.h"
#include "custom/Camera.h"
#include "tiles/Tile.h"

Player::Player()
{
}

Player::Player(const char* n, const MATH::Vec3& pos)
{

	name = n;
	transform = Transform(pos);
	//Always initialize the components that you've inherited with your current gameobject
	//this allows the components to access the transform of of your gameobject
}

Player::~Player()
{
	
}

void Player::Update(const float deltaTime)
{
	//Always update your inherited components and transform
	transform.Update(deltaTime);
	Camera::getInstance()->setPosition(transform.GetPosition());
}

void Player::Render() const
{
	//Render your inherited components

}

void Player::HandleEvents(const SDL_Event& event)
{
	if (event.type == SDL_EventType::SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_w) 
		{
			transform.pos.z += 10.0f * Timer::GetDeltaTime();
		}
		if (event.key.keysym.sym == SDLK_s)
		{
			transform.pos.z -= 10.0f * Timer::GetDeltaTime();
		}
		if (event.key.keysym.sym == SDLK_d)
		{
			transform.pos.x -= 10.0f * Timer::GetDeltaTime();
		}
		if (event.key.keysym.sym == SDLK_a)
		{
			transform.pos.x += 10.0f * Timer::GetDeltaTime();
		}
		if (event.key.keysym.sym == SDLK_SPACE)
		{
			transform.pos.y -= 10.0f * Timer::GetDeltaTime();
		}
		if (event.key.keysym.sym == SDLK_LCTRL)
		{
			transform.pos.y += 10.0f * Timer::GetDeltaTime();
		}
	}
}
