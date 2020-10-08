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
	//collider.colliderShape = Collider3D::shape::AABB;
	//RigidBody3D::Init(this);
	//RigidBody3D::SetAccel(MATH::Vec3(0.0f, -9.8f, 0.0f));

	
	//Always initialize the components that you've inherited with your current gameobject
	//this allows the components to access the transform of of your gameobject
	moveSpeed = 20.0f;
	turnSpeed = 70.0f;
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
	//RigidBody3D::Update(deltaTime);
}

void Player::Render() const
{
}

void Player::HandleEvents(const SDL_Event& event)
{
	const Uint8* keyboard_state_array = SDL_GetKeyboardState(NULL);
    MATH::Vec3 moveDir = Vec3(0.0f, 0.0f, 0.0f);
    
	if (event.type == SDL_EventType::SDL_KEYDOWN)
	{
		//Multi Key Movement controls (From SDL2)
		if (keyboard_state_array[SDL_SCANCODE_W])
		{
            moveDir += transform.Forward();
		}
		if (keyboard_state_array[SDL_SCANCODE_D])
		{
            moveDir += transform.Right();
		}
		if (keyboard_state_array[SDL_SCANCODE_S])
		{
            moveDir += -transform.Forward();
		}
		if (keyboard_state_array[SDL_SCANCODE_A])
		{
            moveDir += -transform.Right();
		}
        if (keyboard_state_array[SDL_SCANCODE_SPACE])
		{
            moveDir += transform.Up();
		}
        if (keyboard_state_array[SDL_SCANCODE_LCTRL])
		{
            moveDir += -transform.Up();
		}
		transform.pos += moveDir * moveSpeed * Timer::GetDeltaTime();
    }
		
	//Rotation controls
	if (event.key.keysym.sym == SDLK_LEFT)
	{
		transform.rotation += Vec3(0.0f, turnSpeed, 0.0f) * Timer::GetDeltaTime();
	}
	if (event.key.keysym.sym == SDLK_RIGHT)
	{
		transform.rotation += Vec3(0.0f, -turnSpeed, 0.0f) * Timer::GetDeltaTime();
	}
	if (event.key.keysym.sym == SDLK_UP)
	{
		transform.rotation += Vec3(turnSpeed, 0.0f, 0.0f) * Timer::GetDeltaTime();
	}
	if (event.key.keysym.sym == SDLK_DOWN)
	{
		transform.rotation += Vec3(-turnSpeed, 0.0f, 0.0f) * Timer::GetDeltaTime();
	}
}
