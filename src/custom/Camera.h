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

	void Update(float deltaTime);

	static Camera* getInstance();
	inline static void removeInstance() { delete instance; instance = nullptr; }
	inline Matrix4& getProjectionMatrix() { return orthoProjMatrix; }
	inline Matrix4& getViewMatrix() { return viewMatrix; }
	inline void setProjectionMatrix(Matrix4 projection_) { orthoProjMatrix = projection_; }
	inline void setViewMatrix(Matrix4 view_) { viewMatrix = view_; }
	inline Vec3 getPosition() { return position; }
	inline void setPosition(Vec3 pos) { position = pos; }
		
private:
	
	static Camera* instance;
	Vec3 position;
	Matrix4 viewMatrix;
	Matrix4 orthoProjMatrix;
	Matrix4 perspecProjMatrix;
	
	
};
#endif 

