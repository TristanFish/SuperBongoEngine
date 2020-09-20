#include "Camera.h"
#include "math/VMath.h"
using namespace MATH;

Camera* Camera::instance;

Camera::Camera()
{

	orthoProjMatrix = MMath::orthographic(-10.0f, 10.0f, -10.0f, 10.0f, -20.0f, 20.0f);
	perspecProjMatrix = MMath::perspective(90.0f, (12.0f / 9.0f), 0.1f, 100.0f);

}

void Camera::Update(float deltaTime)
{
	rotationMatrix = MMath::calcRotationMatrix(rotation);

	viewMatrix = MMath::inverse(MMath::translate(position) * rotationMatrix);

	//std::cout << "Camera position/rotation: " << position << " " << rotation << std::endl;
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
