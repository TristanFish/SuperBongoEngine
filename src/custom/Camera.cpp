#include "Camera.h"

using namespace MATH;

Camera* Camera::instance;

Camera::Camera()
{
	orthoProjMatrix = MMath::orthographic(-10.0f, 10.0f, -10.0f, 10.0f, -20.0f, 20.0f);
	viewMatrix.loadIdentity();
	position = Vec3();
}

void Camera::Update(float deltaTime)
{
	viewMatrix = MMath::translate(-position.x * 0.1f, -position.y * 0.1f, -0.5f);
}

Camera* Camera::getInstance()
{
	if (instance == nullptr)
	{
		instance = new Camera();
	}
	return instance;	
}
