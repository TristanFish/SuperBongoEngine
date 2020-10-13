#ifndef MOUSERAY_H
#define MOUSERAY_H

#include "VMath.h"
#include "Vector.h"
#include "MMath.h"
#include "Camera.h"
class MouseRay
{
private:
	// Current Ray/Direction of the ray
	MATH::Vec3 currentRay;

	// Current position of the mouse on screen
	MATH::Vec2 localMousePos;


	// Converts the local Mouse Position to normal Coordinates 
	MATH::Vec2 GetDeviceCoords(int x_, int y_);

	// Converts clipping Coordinates to eye Coordinates
	MATH::Vec4 GetEyeCoords(MATH::Vec4 clipCoords);

	// Converts eye Coordinates to world Coordinates
	MATH::Vec3 GetWorldCoords(MATH::Vec4 eyeCoords);

	// Does the final calculation and then set's the currentRay
	MATH::Vec3 CalaculateMouseRay();

public:
	MouseRay();
	~MouseRay();

	

	const MATH::Vec3 GetCurrentRay() { return currentRay; }

	void Update(const float deltatime);
	void HandleEvents(const SDL_Event& event);
	



};

#endif