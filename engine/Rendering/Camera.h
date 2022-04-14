#ifndef CAMERA_H
#define CAMERA_H

#include "MMath.h"
#include "core/events/MouseEventListener.h"
#include <sdl/SDL.h>

enum class CameraTypes
{
	Editor,
	Gameplay
};

//!Camera Class
/*! Controls how the players camera acts */
class Camera : public MouseEventListener
{

public:

	Camera();

	void Update(float deltaTime);

	MATH::Matrix4& getProjectionMatrix() { return perspecProjMatrix; }
	MATH::Matrix4& getViewMatrix() { return viewMatrix; }
	MATH::Matrix4& getRotationMatrix() { return rotationMatrix; }
	MATH::Matrix4& getInvNDC() { return invNDC; }
	void setProjectionMatrix(MATH::Matrix4 projection_) { perspecProjMatrix = projection_; }
	void setViewMatrix(MATH::Matrix4 view_) { viewMatrix = view_; }
	MATH::Vec3 getPosition() const { return position; }
	MATH::Vec3 getRotation() const { return MATH::MMath::calcEulerAngles(rotationMatrix) * static_cast<float>(1.0f / DEGREES_TO_RADIANS); }
	void setPosition(MATH::Vec3 pos) { position = pos; }
	void setRotation(MATH::Matrix4 rot) { rotationMatrix = rot; }
	float getNearPlane() const { return nearPlane; }
	float getFarPlane() const { return farPlane; }
	float getAspectRatio() const { return aspect; }
	void setAspectRatio(float a) { aspect = a; }
	MATH::Vec3 GetMouseVector(int x, int y) const;
	void UpdatePerspectiveMatrix();


	MATH::Vec3 Up() const;
	MATH::Vec3 Right() const;
	MATH::Vec3 Forward() const;
	
	CameraTypes cameraType;
	void SetCameraType(CameraTypes type);
	
private:
	
	MATH::Vec3 position;
	MATH::Vec3 rotation;
	MATH::Matrix4 rotationMatrix;
	MATH::Matrix4 viewMatrix;
	MATH::Matrix4 orthoProjMatrix;
	MATH::Matrix4 perspecProjMatrix;
	MATH::Matrix4 invNDC;

	float nearPlane;
	float farPlane;
	
	float zoom;
	float aspect;
	const float panSpeed;
	const float sensitivity;


	//!mouseDown boolean
	/*! Changes when the mouse down state gets changed */
	bool mouseDown;
	//!mouseStart Vector3
	/*! Used for detecting trackball player rotation */
	MATH::Vec3 mouseStart;
	//!mouseEnd Vector3
	/*! Used for detecting trackball player rotation  */
	MATH::Vec3 mouseEnd;
	
	void OnMousePressed(MATH::Vec2 mouse, int buttonType) override;
	void OnMouseReleased(MATH::Vec2 mouse, int buttonType) override;
	void OnMouseScroll(int y) override;
	void OnMouseMove(MATH::Vec2 mouse) override;
	
};
#endif 

