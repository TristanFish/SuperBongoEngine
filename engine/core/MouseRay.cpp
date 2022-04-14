// ReSharper disable CppMemberFunctionMayBeStatic
#include "MouseRay.h"
#include "core/Globals.h"
#include "rendering/Renderer.h"

using namespace MATH;

void MouseRay::HandleEvents(const SDL_Event& event)
{
	localMousePos.x = static_cast<float>(event.button.x);
	localMousePos.y = static_cast<float>(event.button.y);
}


Vec2 MouseRay::GetDeviceCoords(float x_, float y_)
{
	Vec2 mouse;

	Vec2 viewportSize = Renderer::GetInstance()->GetViewport().GetViewportSize();
	mouse.x = (x_ / Globals::Engine::SCREEN_WIDTH - 0.5f) * 2.0f;
	mouse.y = (y_ / Globals::Engine::SCREEN_HEIGHT - 0.5f) * 2.0f;
	mouse.y = -mouse.y;

	return mouse;
}

Vec4 MouseRay::GetEyeCoords(Vec4 clipCoords)
{
	const Matrix4 invertProj = MMath::inverse(CameraManager::GetInstance()->GetCamera()->getProjectionMatrix());

	const Vec4 eyeCoords = invertProj * clipCoords;

	return Vec4(eyeCoords.x,eyeCoords.y, -1.0f, 0.0f);
}

Vec3 MouseRay::GetWorldCoords(Vec4 eyeCoords)
{
	const Matrix4 invertView = MMath::inverse(CameraManager::GetInstance()->GetCamera()->getViewMatrix());

	const Vec4 rayWorld = invertView * eyeCoords;

	Vec3 mouseRay = Vec3(rayWorld.x, rayWorld.y, rayWorld.z);

	mouseRay = VMath::normalize(mouseRay);
	return mouseRay;
}

MouseRay::MouseRay(const Vec3& dir_, const Vec3& origin_)
{
	direction = dir_;
	origin = origin_;
}

void MouseRay::CalculateMouseRay()
{
	origin = CameraManager::GetInstance()->GetCamera()->getPosition();

	const Vec2 mousePos = localMousePos;

	const Vec2 normCoords = GetDeviceCoords(mousePos.x, mousePos.y);

	const Vec4 clipCoords = Vec4(normCoords.x, normCoords.y, -1.0f, 1.0f);
	const Vec4 eyeCorrds = GetEyeCoords(clipCoords);

	const Vec3 worldRay = GetWorldCoords(eyeCorrds);

	direction = worldRay;


	invDir = Vec3(1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z);
}
