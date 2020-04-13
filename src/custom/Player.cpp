#include "Player.h"
#include "core/Timer.h"
#include "custom/Camera.h"

Player::Player(const char* n, const MATH::Vec3& pos)
{


	name = n;
	transform = Transform(pos);

	//Always initialize the components that you've inherited with your current gameobject
	//this allows the components to access the transform of of your gameobject
	RigidBodyComponent::Init(this);
	SpriteComponent::Init(this);

	RigidBodyComponent::setColliderShape(Collider::shape::Circle);
	RigidBodyComponent::ApplyConstantForce(MATH::Vec3(0.0f, -1.0f, 0.0f));
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
	Camera::getInstance()->setPosition(VMath::lerp(Camera::getInstance()->getPosition(), transform.GetPosition(), 0.05f));
}

void Player::Render() const
{
	//Render your inherited components
	RigidBodyComponent::Render();
	SpriteComponent::Render();
}

void Player::HandleEvents(const SDL_Event& event)
{
	if (event.type == SDL_EventType::SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_w) {
			RigidBodyComponent::ApplyImpulseForce(MATH::MMath::rotate(transform.GetRotation().z, MATH::Vec3(0.0f, 0.0f, 1.0f)) 
												* MATH::Vec3(0.0f, 1.0f, 0.0f) * 2.0f);
			RigidBodyComponent::SetIsGrounded(false);

		}
		if (event.key.keysym.sym == SDLK_q)
		{
			RigidBodyComponent::ApplyImpulseTorque(5.0f);
			std::cout << RigidBodyComponent::GetAngVelocity() << std::endl;
		}
		if (event.key.keysym.sym == SDLK_e)
		{
			RigidBodyComponent::ApplyImpulseTorque(-5.0f);
			std::cout << RigidBodyComponent::GetAngVelocity() << std::endl;
		}
		if (event.key.keysym.sym == SDLK_a)
		{
			if (RigidBodyComponent::GetIsGrounded())
			{
				RigidBodyComponent::SetVelocity(Vec3(-2.0, 0.0, 0.0));
			}
		}
		if (event.key.keysym.sym == SDLK_d)
		{
			if (RigidBodyComponent::GetIsGrounded())
			{
				RigidBodyComponent::SetVelocity(Vec3(2.0, 0.0, 0.0));
			}
		}
		if (event.key.keysym.sym == SDLK_SPACE)
		{
			if (RigidBodyComponent::GetIsGrounded())
			{
				RigidBodyComponent::SetVelocity(Vec3(RigidBodyComponent::GetVelocity().x, 2.0, 0.0));
				RigidBodyComponent::SetIsGrounded(false);
			}
		}
	}

	if (event.type == SDL_EventType::SDL_KEYUP)
	{
		if (event.key.keysym.sym == SDLK_a)
		{
			if (RigidBodyComponent::GetIsGrounded())
			{
				RigidBodyComponent::SetVelocity(Vec3(0.0, 0.0, 0.0));
				RigidBodyComponent::SetAccel(Vec3(0.0, 0.0, 0.0));
			}
		}
		if (event.key.keysym.sym == SDLK_d)
		{
			if (RigidBodyComponent::GetIsGrounded())
			{
				RigidBodyComponent::SetVelocity(Vec3(0.0, 0.0, 0.0));
				RigidBodyComponent::SetAccel(Vec3(0.0, 0.0, 0.0));
			}
		}
	}

	RigidBodyComponent::HandleEvents(event);
	SpriteComponent::HandleEvents(event);
}

void Player::OnCollisionEnter()
{
}
