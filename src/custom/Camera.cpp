#include "Camera.h"

using namespace MATH;

Camera::Camera()
{
	orthoProjMatrix = (MMath::orthographic(1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f));
	viewProjectionMatrix = orthoProjMatrix * viewMatrix;
}