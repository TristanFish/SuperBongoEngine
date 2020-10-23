#include "MouseRay.h"
#include "core/Globals.h"
MouseRay::MouseRay()
{
	
}

MouseRay::~MouseRay()
{
	delete ray;
	ray = nullptr;
}




void MouseRay::HandleEvents(const SDL_Event& event)
{
	localMousePos.x = event.button.x;
	localMousePos.y = event.button.y;
}

MATH::Vec2 MouseRay::GetDeviceCoords(int x_, int y_)
{
	MATH::Vec2 mouse;
	mouse.x = (2.0f * x_) / Globals::SCREEN_WIDTH - 1.0f;
	mouse.y = (2.0f * y_) / Globals::SCREEN_HEIGHT - 1.0f;
	mouse.y = -mouse.y;

	return mouse;
}

MATH::Vec4 MouseRay::GetEyeCoords(MATH::Vec4 clipCoords)
{
	MATH::Matrix4 invertProj = MATH::MMath::inverse(Camera::getInstance()->getProjectionMatrix());

	MATH::Vec4 eyeCoords = invertProj * clipCoords;

	return MATH::Vec4(eyeCoords.x,eyeCoords.y, -1.0f, 0.0f);
}

MATH::Vec3 MouseRay::GetWorldCoords(MATH::Vec4 eyeCoords)
{
	MATH::Matrix4 invertView = MATH::MMath::inverse(Camera::getInstance()->getViewMatrix());

	MATH::Vec4 rayWorld = invertView * eyeCoords;

	MATH::Vec3 mouseRay = MATH::Vec3(rayWorld.x, rayWorld.y, rayWorld.z);

	mouseRay = MATH::VMath::normalize(mouseRay);
	return mouseRay;
}

void MouseRay::CalaculateMouseRay()
{
	ray->Origin = Camera::getInstance()->getPosition();

	MATH::Vec2 mousePos = localMousePos;

	MATH::Vec2 normCoords = GetDeviceCoords(mousePos.x, mousePos.y);

	MATH::Vec4 clipCoords = MATH::Vec4(normCoords.x, normCoords.y, -1.0f, 1.0f);
	MATH::Vec4 eyeCorrds = GetEyeCoords(clipCoords);

	MATH::Vec3 worldRay = GetWorldCoords(eyeCorrds);

	ray->Direction = worldRay;

	invDir = MATH::Vec3(1.0f / ray->Direction.x, 1.0f / ray->Direction.y, 1.0f / ray->Direction.z);

}
