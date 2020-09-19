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
	Camera::getInstance()->setRotation(transform.GetRotation());
}

void Player::Render() const
{
	//Render your inherited components

}

void Player::HandleEvents(const SDL_Event& event)
{
	if (event.type == SDL_EventType::SDL_KEYDOWN)
	{
		//Movement controls
		if (event.key.keysym.sym == SDLK_w) 
		{
			transform.pos += transform.Forward() * 10.0f * Timer::GetDeltaTime();
		}
		if (event.key.keysym.sym == SDLK_s)
		{
			transform.pos += -transform.Forward() * 10.0f * Timer::GetDeltaTime();
		}
		if (event.key.keysym.sym == SDLK_d)
		{
			transform.pos += transform.Right() * 10.0f * Timer::GetDeltaTime();
		}
		if (event.key.keysym.sym == SDLK_a)
		{
			transform.pos += -transform.Right() * 10.0f * Timer::GetDeltaTime();
		}
		if (event.key.keysym.sym == SDLK_SPACE)
		{
			transform.pos += transform.Up() * 10.0f * Timer::GetDeltaTime();
		}
		if (event.key.keysym.sym == SDLK_LCTRL)
		{
			transform.pos += -transform.Up() * 10.0f * Timer::GetDeltaTime();
		}

		//Rotation controls
		if (event.key.keysym.sym == SDLK_LEFT)
		{
			transform.rotation.y +=  50.0f * Timer::GetDeltaTime();
		}
		if (event.key.keysym.sym == SDLK_RIGHT)
		{
			transform.rotation.y += -50.0f * Timer::GetDeltaTime();
		}
		if (event.key.keysym.sym == SDLK_UP)
		{
			transform.rotation.x +=  50.0f * Timer::GetDeltaTime();
		}
		if (event.key.keysym.sym == SDLK_DOWN)
		{
			transform.rotation.x += -50.0f * Timer::GetDeltaTime();
		}
		
	}
}
