#include "Camera.h"

using namespace MATH;

Camera::Camera()
{
	orthoProjMatrix = (MMath::orthographic(-3.0f, 1.0f, -3.0f, 1.0f, -1.0f, 1.0f));
	viewMatrix.loadIdentity();
	

}