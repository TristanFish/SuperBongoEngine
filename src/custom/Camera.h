#ifndef CAMERA_H
#define CAMERA_H

#include "Matrix.h"
#include "MMath.h"
#include "VMath.h"

using namespace MATH;

class Camera
{
	Matrix4 viewMatrix;
	Matrix4 orthoMatrix;
	Matrix4 perspecMatrix;
};
#endif 

