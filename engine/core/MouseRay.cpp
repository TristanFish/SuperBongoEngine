// ReSharper disable CppMemberFunctionMayBeStatic
#include "MouseRay.h"
#include "core/Globals.h"


void MouseRay::HandleEvents(const SDL_Event& event)
{
	localMousePos.x = static_cast<float>(event.button.x);
	localMousePos.y = static_cast<float>(event.button.y);
}


MATH::Vec2 MouseRay::GetDeviceCoords(float x_, float y_)
{
	MATH::Vec2 mouse;
	mouse.x = (2.0f * x_) / Globals::SCREEN_WIDTH - 1.0f;
	mouse.y = (2.0f * y_) / Globals::SCREEN_HEIGHT - 1.0f;
	mouse.y = -mouse.y;

	return mouse;
}

MATH::Vec4 MouseRay::GetEyeCoords(MATH::Vec4 clipCoords)
{
	const MATH::Matrix4 invertProj = MATH::MMath::inverse(Camera::getInstance()->getProjectionMatrix());

	const MATH::Vec4 eyeCoords = invertProj * clipCoords;

	return MATH::Vec4(eyeCoords.x,eyeCoords.y, -1.0f, 0.0f);
}

MATH::Vec3 MouseRay::GetWorldCoords(MATH::Vec4 eyeCoords)
{
	const MATH::Matrix4 invertView = MATH::MMath::inverse(Camera::getInstance()->getViewMatrix());

	const MATH::Vec4 rayWorld = invertView * eyeCoords;

	MATH::Vec3 mouseRay = MATH::Vec3(rayWorld.x, rayWorld.y, rayWorld.z);

	mouseRay = MATH::VMath::normalize(mouseRay);
	return mouseRay;
}

void MouseRay::CalaculateMouseRay()
{
	ray.origin = Camera::getInstance()->getPosition();

	const MATH::Vec2 mousePos = localMousePos;

	const MATH::Vec2 normCoords = GetDeviceCoords(mousePos.x, mousePos.y);

	const MATH::Vec4 clipCoords = MATH::Vec4(normCoords.x, normCoords.y, -1.0f, 1.0f);
	const MATH::Vec4 eyeCorrds = GetEyeCoords(clipCoords);

	const MATH::Vec3 worldRay = GetWorldCoords(eyeCorrds);

	ray.direction = worldRay;

	invDir = MATH::Vec3(1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z);

}
