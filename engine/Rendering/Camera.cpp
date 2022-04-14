#include "Camera.h"
#include "math/VMath.h"
#include "core/Globals.h"
#include <glew/glew.h>


#include "core/events/InputManager.h"
#include "core/Timer.h"
using namespace MATH;

Camera::Camera() : nearPlane(0.1f), farPlane(300.0f), zoom(60.0f), panSpeed(20.0f), sensitivity(80.0f), mouseDown(false)
{
	position.z = 100.0f;

	aspect = static_cast<float>(Globals::Engine::SCREEN_WIDTH) / static_cast<float>(Globals::Engine::SCREEN_HEIGHT);
	orthoProjMatrix = MMath::orthographic(-10.0f, 10.0f, -10.0f, 10.0f, -20.0f, 20.0f);
	perspecProjMatrix = MMath::perspective(zoom, aspect, nearPlane, farPlane);
	
	rotation.x = 3.14f;
	rotation.y = 1.57f;
	Vec3 direction;
	direction.x = cos(rotation.y) * cos(rotation.x);
	direction.y = sin(rotation.x);
	direction.z = sin(rotation.y) * cos(rotation.x);
	direction = VMath::normalize(direction);
	rotationMatrix = MMath::lookAt(Vec3(), direction, Vec3(0.0f, 1.0f, 0.0f));

	invNDC = MMath::inverse(MMath::viewportNDC(829, 552));
	viewMatrix = MMath::inverse(MMath::translate(position) * rotationMatrix);
}

void Camera::Update(float deltaTime)
{
	//rotationMatrix = MMath::calcRotationMatrix(rotation);
	#pragma region Movement Controls

	if(cameraType == CameraTypes::Editor)
	{
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
	}
#pragma endregion

	viewMatrix = MMath::inverse(MMath::translate(position) * rotationMatrix);
}

Vec3 Camera::GetMouseVector(int x, int y) const
{
	const MATH::Vec3 mousePosition = MATH::Vec3(static_cast<float>(x), static_cast<float>(y), 0.0f);
	MATH::Vec3 v = invNDC * mousePosition;

	return v;
}

void Camera::UpdatePerspectiveMatrix()
{
	perspecProjMatrix = MMath::perspective(zoom, aspect, nearPlane, farPlane);
}

void Camera::OnMousePressed(MATH::Vec2 mouse, int buttonType)
{
	if(buttonType == SDL_BUTTON_RIGHT)
	{
		mouseDown = true;
		mouseStart = GetMouseVector(static_cast<int>(mouse.x), static_cast<int>(mouse.y));
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
	if (!Renderer::GetInstance()->GetViewport().GetIsMouseHovered())
		return;

	zoom -= y;

	if(zoom < 5.0f)
	{
		zoom = 5.0f;
	}

	if(zoom > 135.0f)
	{
		zoom = 135.0f;
	}
	
	UpdatePerspectiveMatrix();
}

void Camera::OnMouseMove(MATH::Vec2 mouse)
{
	if (!Renderer::GetInstance()->GetViewport().GetIsMouseHovered())
		return;

	if (mouseDown == false)
	{
		return;
	}

	mouseEnd = GetMouseVector(static_cast<int>(mouse.x), static_cast<int>(mouse.y));

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

void Camera::SetCameraType(CameraTypes type)
{
	cameraType = type;
}