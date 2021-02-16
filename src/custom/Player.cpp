#include "Player.h"
#include "core/Timer.h"
#include "custom/Rendering/Camera.h"
#include "core/InputManager.h"
#include "math/Quaternion.h"

Player::Player() : moveSpeed(20.0f), turnSpeed(70.0f), RunSpeed(10.0f)
{
}

Player::Player(const char* n, const MATH::Vec3& pos) :  moveSpeed(20.0f), turnSpeed(70.0f), RunSpeed(10.0f)
{

	name = n;
	transform = Transform(pos);
	transform.rotation.y = -90.0f;
	AudioListenerComponent::Init(this);
	
}

Player::~Player()
{
	
}

void Player::Update(const float deltaTime)
{
	//Always update your inherited components and transform
#pragma region Movement Controls

	MATH::Vec3 moveDir = Vec3(0.0f, 0.0f, 0.0f);

	//Movement controls
	if (InputManager::GetInstance()->GetKey(SDLK_w))
	{
		moveDir += transform.Forward();
	}
	if (InputManager::GetInstance()->GetKey(SDLK_s))
	{
		moveDir += -transform.Forward();
	}
	if (InputManager::GetInstance()->GetKey(SDLK_a))
	{
		moveDir += -transform.Right();
	}
	if (InputManager::GetInstance()->GetKey(SDLK_d))
	{
		moveDir += transform.Right();
	}
	if (InputManager::GetInstance()->GetKey(SDLK_SPACE))
	{
		moveDir += transform.Up();
	}
	if (InputManager::GetInstance()->GetKey(SDLK_LCTRL))
	{
		moveDir += -transform.Up();
	}
	transform.pos += moveDir * moveSpeed * deltaTime;
#pragma endregion

	//transform.Update(deltaTime);
	Camera::getInstance()->setPosition(transform.GetPosition());
	Camera::getInstance()->setRotation(transform.rotationMatrix);
	//Dont really need to update this but once models are moving their sound posititions will have to follow
	AudioListenerComponent::Update(deltaTime);
	//RigidBody3D::Update(deltaTime);
	//MeshRenderer::Update(deltaTime);
}

void Player::Render() const
{
	//MeshRenderer::Render();
}

Vec3 GetMouseVector(int x, int y)
{
	Vec3 mousePosition = Vec3(static_cast<float>(x), static_cast<float>(y), 0.0f);
	Vec3 v = Camera::getInstance()->getInvNDC() * mousePosition;

	return v;
}

void Player::OnMouseMove(int x, int y)
{
	if (mouseDown == false)
	{
		return;
	}

	mouseEnd = GetMouseVector(x, y);

	transform.rotation.x += (mouseEnd.y - mouseStart.y) * turnSpeed * Timer::GetDeltaTime();
	transform.rotation.y += (mouseEnd.x - mouseStart.x) * turnSpeed * Timer::GetDeltaTime();

	if (transform.rotation.x > 89.0f)
	{
		transform.rotation.x = 89.0f;
	}
	if (transform.rotation.x < -89.0f)
	{
		transform.rotation.x = -89.0f;
	}
	Vec3 direction;
	direction.x = cos(transform.rotation.y) * cos(transform.rotation.x);
	direction.y = sin(transform.rotation.x);
	direction.z = sin(transform.rotation.y) * cos(transform.rotation.x);
	direction = VMath::normalize(direction);

	transform.rotationMatrix = MMath::lookAt(Vec3(), direction, Vec3(0.0f, 1.0f, 0.0f));
	/// reset for the next mousemove
	mouseStart = mouseEnd;
}

void Player::HandleEvents(const SDL_Event& event)
{
	//MeshRenderer::HandleEvents(event);
#pragma region Rotation Controls
	//Rotation controls
	if (InputManager::GetInstance()->GetKey(SDLK_LALT))
	{
		if (event.type == SDL_EventType::SDL_MOUSEBUTTONDOWN)
		{
			mouseDown = true;
			mouseStart = GetMouseVector(event.button.x, event.button.y);
		}
		else if (event.type == SDL_EventType::SDL_MOUSEBUTTONUP)
		{
			mouseDown = false;
		}
		if (event.type == SDL_EventType::SDL_MOUSEMOTION &&
			SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			OnMouseMove(event.button.x, event.button.y);
		}
	}
#pragma endregion
}


