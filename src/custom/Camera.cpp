#include "Camera.h"

using namespace MATH;

Camera* Camera::instance;

Camera::Camera()
{
	position = MATH::Vec3(0.0f, 0.0f, -10.0f);
	orthoProjMatrix = MMath::orthographic(-10.0f, 10.0f, -10.0f, 10.0f, -20.0f, 20.0f);
	perspecProjMatrix = MMath::perspective(90.0f, (12.0f / 9.0f), 0.1f, 100.0f);
	viewMatrix = MMath::lookAt(position, MATH::Vec3(), MATH::Vec3(0.0f, 1.0f, 0.0f));
}

void Camera::Update(float deltaTime)
{
	viewMatrix = MMath::translate(position.x, position.y, position.z);
}

void Camera::HandleEvents(const SDL_Event& event)
{

}

Camera* Camera::getInstance()
{
	if (instance == nullptr)
	{
		instance = new Camera();
	}
	return instance;	
}
