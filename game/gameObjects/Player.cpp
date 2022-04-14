#include "Player.h"
#include "core/Timer.h"
#include "Rendering/CameraManager.h"
#include "core/events/InputManager.h"
#include "math/Quaternion.h"

using namespace MATH;


Player::Player(const std::string& name_, const MATH::Vec3& pos) :  moveSpeed(20.0f), turnSpeed(80.0f)
{	
	name = name_;
	transform = Transform(pos);
	transform.SetRot(Quaternion::EulerToQuat(Vec3(0.0, -90.0f, 0.0f)));

	AddComponent<MeshRenderer>()->LoadModel("Cube.obj");
	AddComponent<RigidBody3D>();

	GetComponent<MeshRenderer>()->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");
	GetComponent<MeshRenderer>()->active = false;

	GetComponent<RigidBody3D>()->ApplyConstantForce(MATH::Vec3(0.0, -9.8, 0.0));
}


void Player::PostInit()
{
	GetComponent<RigidBody3D>()->ConstructCollider(ColliderType::OBB);

	GameObject::PostInit();
}

void Player::Update(const float deltaTime)
{
	GameObject::Update(deltaTime);
	//Always update your inherited components and transform
#pragma region Movement Controls

	Vec3 moveDir = Vec3();
	Vec3 turnDir = Vec3();

	Vec3 forward = CameraManager::GetInstance()->GetCamera()->Forward();
	Vec3 right = CameraManager::GetInstance()->GetCamera()->Right();

	forward = VMath::normalize(Vec3(forward.x, 0.0f, forward.z));
	right = VMath::normalize(Vec3(right.x, 0.0f, right.z));

	//Movement controls
	if (InputManager::GetInstance()->GetKey(SDLK_w))
	{
		moveDir += forward;
	}
	if (InputManager::GetInstance()->GetKey(SDLK_s))
	{
		moveDir += -forward;
	}
	if (InputManager::GetInstance()->GetKey(SDLK_a))
	{
		moveDir += -right;
	}
	if (InputManager::GetInstance()->GetKey(SDLK_d))
	{
		moveDir += right;
	}
	if (InputManager::GetInstance()->GetKey(SDLK_SPACE) && HasLanded)
	{
		GetComponent<RigidBody3D>()->ApplyImpulseForce(Vec3(0.0f, 1000.0f, 0.0f));
		HasLanded = false;
	}

	transform.SetPos(transform.GetPosition() + (moveDir * moveSpeed * deltaTime));
	CameraManager::GetInstance()->GetCamera()->setPosition(transform.GetPosition());

	
#pragma endregion

}

void Player::HandleEvents(const SDL_Event& event)
{

}

void Player::OnCollisionEnter(Collider3D& otherCollider)
{
	HasLanded = true;
}
