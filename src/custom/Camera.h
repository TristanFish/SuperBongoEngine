#ifndef CAMERA_H
#define CAMERA_H

#include "MMath.h"
#include "core/MouseEventListener.h"
#include <sdl/SDL.h>


//!Camera Class
/*! Controls how the players camera acts */
class Camera : public MouseEventListener
{

public:

	Camera();

	void Update(float deltaTime);


	static Camera* getInstance();
	inline static void removeInstance() { delete instance; instance = nullptr; }
	inline MATH::Matrix4& getProjectionMatrix() { return perspecProjMatrix; }
	inline MATH::Matrix4& getViewMatrix() { return viewMatrix; }
	inline MATH::Matrix4& getRotationMatrix() { return rotationMatrix; }
	inline MATH::Matrix4& getInvNDC() { return invNDC; }
	inline void setProjectionMatrix(MATH::Matrix4 projection_) { perspecProjMatrix = projection_; }
	inline void setViewMatrix(MATH::Matrix4 view_) { viewMatrix = view_; }
	inline MATH::Vec3 getPosition() const { return position; }
	inline MATH::Vec3 getRotation() const { return MATH::MMath::calcEulerAngles(rotationMatrix) * (1.0f / DEGREES_TO_RADIANS); }
	inline void setPosition(MATH::Vec3 pos) { position = pos; }
	inline void setRotation(MATH::Matrix4 rot) { rotationMatrix = rot; }

	MATH::Vec3 GetMouseVector(int x, int y) const;

	
private:
	
	static Camera* instance;
	MATH::Vec3 position;
	MATH::Vec3 rotation;
	MATH::Matrix4 rotationMatrix;
	MATH::Matrix4 viewMatrix;
	MATH::Matrix4 orthoProjMatrix;
	MATH::Matrix4 perspecProjMatrix;
	MATH::Matrix4 invNDC;

	float zoom;
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

	MATH::Vec3 Up() const;
	MATH::Vec3 Right() const;
	MATH::Vec3 Forward() const;
	
};
#endif 

