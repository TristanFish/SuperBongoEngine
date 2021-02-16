#include "Camera.h"
#include "math/VMath.h"
#include "core/Globals.h"
#include <glew/glew.h>
using namespace MATH;

Camera* Camera::instance;

Camera::Camera()
{

	orthoProjMatrix = MMath::orthographic(-10.0f, 10.0f, -10.0f, 10.0f, -20.0f, 20.0f);
	perspecProjMatrix = MMath::perspective(60.0f, (static_cast<float>(Globals::SCREEN_WIDTH) / static_cast<float>(Globals::SCREEN_HEIGHT)), 0.1f, 150.0f);
	
	int viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);

	invNDC = MMath::inverse(MMath::viewportNDC(viewport[2], viewport[3]));
}

void Camera::Update(float deltaTime)
{
	//rotationMatrix = MMath::calcRotationMatrix(rotation);

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
