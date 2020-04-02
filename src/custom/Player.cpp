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
	Camera::getInstance()->setPosition(VMath::lerp(Camera::getInstance()->getPosition(), transform.GetPosition(), 0.2f));
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
		if (event.key.keysym.sym == SDLK_SPACE) {
			RigidBodyComponent::ApplyImpulseForce(MATH::MMath::rotate(transform.GetRotation().z, MATH::Vec3(0.0f, 0.0f, 1.0f)) 
												* MATH::Vec3(0.0f, 1.0f, 0.0f) * 2.0f);
		}
		if (event.key.keysym.sym == SDLK_k)
		{
			Timer::SetTimeScale(1.0f);
		}
		if (event.key.keysym.sym == SDLK_l)
		{
			Timer::SetTimeScale(0.5f);
		}
		if (event.key.keysym.sym == SDLK_a)
		{
			RigidBodyComponent::ApplyImpulseTorque(5.0f);
			std::cout << RigidBodyComponent::GetAngVelocity() << std::endl;
		}
		if (event.key.keysym.sym == SDLK_d)
		{
			RigidBodyComponent::ApplyImpulseTorque(-5.0f);
			std::cout << RigidBodyComponent::GetAngVelocity() << std::endl;
		}
	}

	RigidBodyComponent::HandleEvents(event);
	SpriteComponent::HandleEvents(event);
}

void Player::OnCollisionEnter()
{
}
