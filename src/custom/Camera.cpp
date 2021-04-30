#include "Camera.h"
#include "math/VMath.h"
#include "core/Globals.h"
#include <glew/glew.h>


#include "core/InputManager.h"
#include "core/Timer.h"
using namespace MATH;

Camera* Camera::instance;

Camera::Camera() : nearPlane(0.1f), farPlane(150.0f), zoom(60.0f), panSpeed(20.0f), sensitivity(80.0f), mouseDown(false)
{

	orthoProjMatrix = MMath::orthographic(-10.0f, 10.0f, -10.0f, 10.0f, -20.0f, 20.0f);
	perspecProjMatrix = MMath::perspective(zoom, (static_cast<float>(Globals::SCREEN_WIDTH) / static_cast<float>(Globals::SCREEN_HEIGHT)), nearPlane, farPlane);
	
	int viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);

	rotation.x = 3.14f;

	invNDC = MMath::inverse(MMath::viewportNDC(viewport[2], viewport[3]));
	rotationMatrix = MMath::lookAt(position, Forward(), Vec3(0.0f, 1.0f, 0.0f));
}

void Camera::Update(float deltaTime)
{
	//rotationMatrix = MMath::calcRotationMatrix(rotation);
	#pragma region Movement Controls

	MATH::Vec3 moveDir = Vec3(0.0f, 0.0f, 0.0f);

	//Movement controls
	if (InputManager::GetInstance()->GetKey(SDLK_w))
	{
		moveDir += Forward();
	}
	if (InputManager::GetInstance()->GetKey(SDLK_s))
	{
		moveDir += -Forward();
	}
	if (InputManager::GetInstance()->GetKey(SDLK_a))
	{
		moveDir += -Right();
	}
	if (InputManager::GetInstance()->GetKey(SDLK_d))
	{
		moveDir += Right();
	}
	if (InputManager::GetInstance()->GetKey(SDLK_SPACE))
	{
		moveDir += Up();
	}
	if (InputManager::GetInstance()->GetKey(SDLK_LCTRL))
	{
		moveDir += -Up();
	}
	position += moveDir * panSpeed * deltaTime;
#pragma endregion

	viewMatrix = MMath::inverse(MMath::translate(position) * rotationMatrix);

}

Camera* Camera::getInstance()
{
	if (instance == nullptr)
	{
		instance = new Camera();
	}
	return instance;	
}

Vec3 Camera::GetMouseVector(int x, int y) const
{
	const MATH::Vec3 mousePosition = MATH::Vec3(static_cast<float>(x), static_cast<float>(y), 0.0f);
	MATH::Vec3 v = invNDC * mousePosition;

	return v;
}

void Camera::OnMousePressed(MATH::Vec2 mouse, int buttonType)
{
	if(buttonType == SDL_BUTTON_RIGHT)
	{
		mouseDown = true;
		mouseStart = GetMouseVector(mouse.x, mouse.y);
	}
}

void Camera::OnMouseReleased(MATH::Vec2 mouse, int buttonType)
{
	if(buttonType == SDL_BUTTON_RIGHT)
	{
		mouseDown = false;
	}
}

void Camera::OnMouseScroll(int y)
{
	zoom -= y;

	if(zoom < 5.0f)
	{
		zoom = 5.0f;
	}

	if(zoom > 135.0f)
	{
		zoom = 135.0f;
	}
	
	perspecProjMatrix = MMath::perspective(zoom, (static_cast<float>(Globals::SCREEN_WIDTH) / static_cast<float>(Globals::SCREEN_HEIGHT)), 0.1f, 150.0f);
}

void Camera::OnMouseMove(MATH::Vec2 mouse)
{
	if (mouseDown == false)
	{
		return;
	}

	mouseEnd = GetMouseVector(mouse.x, mouse.y);

	rotation.x += (mouseEnd.y - mouseStart.y) * sensitivity * Timer::GetDeltaTime();
	rotation.y += (mouseEnd.x - mouseStart.x) * sensitivity * Timer::GetDeltaTime();

	if (rotation.x < 1.5708f)
	{
		rotation.x = 1.5708f;
	}
	if (rotation.x > 4.71f)
	{
		rotation.x = 4.71f;
	}

	std::cout << rotation.x << std::endl;

	if(rotation.y > 6.28319f)
	{
		rotation.y -= 6.28319f;
	}
	if(rotation.y < 0.0f)
	{
		rotation.y += 6.28319f;
	}
	
	Vec3 direction;
	direction.x = cos(rotation.y) * cos(rotation.x);
	direction.y = sin(rotation.x);
	direction.z = sin(rotation.y) * cos(rotation.x);
	direction = VMath::normalize(direction);

	rotationMatrix = MMath::lookAt(Vec3(), direction, Vec3(0.0f, 1.0f, 0.0f));
	/// reset for the next mousemove
	mouseStart = mouseEnd;
}

Vec3 Camera::Up() const
{
	return rotationMatrix * MATH::Vec3(0.0f, 1.0f, 0.0f);
}

Vec3 Camera::Right() const
{
	return rotationMatrix * MATH::Vec3(1.0f, 0.0f, 0.0f);
}

Vec3 Camera::Forward() const
{
	return rotationMatrix * MATH::Vec3(0.0f, 0.0f, -1.0f);
}
