#include "Player.h"
#include "core/Timer.h"
#include "custom/Camera.h"
#include "tiles/Tile.h"

Player::Player()
{
}

Player::Player(const char* n, const MATH::Vec3& pos) : MeshRenderer("resources/models/Cube.fbx")
{

	name = n;
	transform = Transform(pos);
	collider.colliderShape = Collider3D::shape::AABB;
	RigidBody3D::Init(this);
	RigidBody3D::SetAccel(MATH::Vec3(0.0f, -9.8f, 0.0f));
	MeshRenderer::Init(this);
	MeshRenderer::CreateShader("src/graphics/shaders/DefaultVert.glsl", "src/graphics/shaders/DefaultFrag.glsl");

	
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
	MeshRenderer::Update(deltaTime);
	RigidBody3D::Update(deltaTime);
}

void Player::Render() const
{
	//MeshRenderer::Render();
}

void Player::HandleEvents(const SDL_Event& event)
{
	MeshRenderer::HandleEvents(event);
	if (event.type == SDL_EventType::SDL_KEYDOWN)
	{
		//Movement controls
		if (event.key.keysym.sym == SDLK_w) 
		{
			transform.pos += transform.Forward() * 20.0f * Timer::GetDeltaTime();
		}
		if (event.key.keysym.sym == SDLK_s)
		{
			transform.pos += -transform.Forward() * 20.0f * Timer::GetDeltaTime();
		}
		if (event.key.keysym.sym == SDLK_d)
		{
			transform.pos += transform.Right() * 20.0f * Timer::GetDeltaTime();
		}
		if (event.key.keysym.sym == SDLK_a)
		{
			transform.pos += -transform.Right() * 20.0f * Timer::GetDeltaTime();
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
			transform.rotation.y +=  80.0f * Timer::GetDeltaTime();
		}
		if (event.key.keysym.sym == SDLK_RIGHT)
		{
			transform.rotation.y += -80.0f * Timer::GetDeltaTime();
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

void Player::OnCollisionEnter(RigidBody3D& otherBody)
{

}
