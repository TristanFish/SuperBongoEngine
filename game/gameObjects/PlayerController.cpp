#include "PlayerController.h"


#include "../engine/components/AudioListenerComponent.h"
#include "../engine/components/3D/MeshRenderer.h"
#include "../engine/components/3D/RigidBody3D.h"

PlayerController::PlayerController(const std::string& name_, const MATH::Vec3& pos) : moveSpeed(20.0f), turnSpeed(80.0f)
{
	name = name_;
	transform.SetPos(pos);

	AddComponent<AudioListenerComponent>();
	AddComponent<RigidBody3D>();

	AddComponent<MeshRenderer>()->LoadModel("Cube.obj");
	GetComponent<MeshRenderer>()->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");
	GetComponent<MeshRenderer>()->active = false;

	GetComponent<RigidBody3D>()->ApplyConstantForce(MATH::Vec3(0.0, -9.8, 0.0));
}

void PlayerController::Update(const float deltaTime)
{
	GameObject::Update(deltaTime);

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
}

void PlayerController::HandleEvents(const SDL_Event& event)
{
}
