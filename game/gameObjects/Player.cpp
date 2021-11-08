#include "Player.h"
#include "core/Timer.h"
#include "Rendering/Camera.h"
#include "core/events/InputManager.h"
#include "math/Quaternion.h"

using namespace MATH;


Player::Player(std::string name_, const MATH::Vec3& pos) :  moveSpeed(20.0f), turnSpeed(80.0f)
{
	AddComponent<AudioListenerComponent>();
	
	name = name_;
	transform = Transform(pos);
	transform.rotation.EulerToQuat(Vec3(0.0, -90.0f, 0.0f));
}


void Player::Update(const float deltaTime)
{
	GameObject::Update(deltaTime);
	//Always update your inherited components and transform
#pragma region Movement Controls

	//Vec3 moveDir = Vec3(0.0f, 0.0f, 0.0f);

	////Movement controls
	//if (InputManager::GetInstance()->GetKey(SDLK_w))
	//{
	//	moveDir += transform.Forward();
	//}
	//if (InputManager::GetInstance()->GetKey(SDLK_s))
	//{
	//	moveDir += -transform.Forward();
	//}
	//if (InputManager::GetInstance()->GetKey(SDLK_a))
	//{
	//	moveDir += -transform.Right();
	//}
	//if (InputManager::GetInstance()->GetKey(SDLK_d))
	//{
	//	moveDir += transform.Right();
	//}
	//if (InputManager::GetInstance()->GetKey(SDLK_SPACE))
	//{
	//	moveDir += transform.Up();
	//}
	//if (InputManager::GetInstance()->GetKey(SDLK_LCTRL))
	//{
	//	moveDir += -transform.Up();
	//}
	//transform.pos += moveDir * moveSpeed * deltaTime;
#pragma endregion

}


void Player::HandleEvents(const SDL_Event& event)
{

}