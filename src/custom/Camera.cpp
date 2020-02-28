#include "Camera.h"

using namespace MATH;

Camera* Camera::instance;

Camera::Camera()
{
	orthoProjMatrix = (MMath::orthographic(-5.0f, 5.0f, -5.0f, 5.0f, -1.0f, 1.0f));
	viewMatrix.loadIdentity();
}

Camera* Camera::getInstance()
{
	if (instance == nullptr)
	{
		instance = new Camera();
	}
	return instance;	
}
