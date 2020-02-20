#ifndef CAMERA_H
#define CAMERA_H

#include "Matrix.h"
#include "MMath.h"
#include "VMath.h"

using namespace MATH;
class Camera
{
public:

	Camera();
	static Camera* getInstance() { static Camera* camera = new Camera(); return camera;}
	inline Matrix4& getProjectionMatrix() { return orthoProjMatrix; }
	inline Matrix4& getViewMatrix() { return viewMatrix; }
	inline void setProjectionMatrix(Matrix4 projection_) { orthoProjMatrix = projection_; }
	inline void setViewMatrix(Matrix4 view_) { viewMatrix = view_; }
		
private:
	

	Matrix4 viewMatrix;
	Matrix4 orthoProjMatrix;
	Matrix4 perspecProjMatrix;
	
	
};
#endif 

