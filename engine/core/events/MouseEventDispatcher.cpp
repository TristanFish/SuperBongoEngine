#include "MouseEventDispatcher.h"
#include "core/Globals.h"
#include "core/Logger.h"

using namespace MATH;

bool MouseEventDispatcher::firstUpdate = true;
std::vector<MouseEventListener*> MouseEventDispatcher::mouseEventListeners;
Vec2 MouseEventDispatcher::mousePosition = Vec2();
Vec2 MouseEventDispatcher::prevPosition = Vec2();

MouseEventDispatcher::~MouseEventDispatcher()
{
	mouseEventListeners.clear();
}

void MouseEventDispatcher::RegisterListener(MouseEventListener* listener)
{
	mouseEventListeners.push_back(listener);
}

void MouseEventDispatcher::UnRegisterListener(MouseEventListener* listener)
{
	auto it = std::find(mouseEventListeners.begin(), mouseEventListeners.end(), listener);

	if(it != mouseEventListeners.end())
	{
		mouseEventListeners.erase(it);
	}
}


void MouseEventDispatcher::Update(SDL_Event e)
{
	switch (e.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		UpdateMousePosition();
		NotifyMousePressed(e.button.button);
		break;
	case SDL_MOUSEBUTTONUP:
		UpdateMousePosition();
		NotifyMouseReleased(e.button.button);
		break;
	case SDL_MOUSEMOTION:
		UpdateMousePosition();
		NotifyMouseMove();
		break;
	case SDL_MOUSEWHEEL:
		UpdateMousePosition();
		NotifyMouseScroll(e.wheel.y);
		break;
	default:
		break;
	}
}

void MouseEventDispatcher::NotifyMousePressed(int buttonType)
{
	for (auto& listener : mouseEventListeners)
	{
		listener->OnMousePressed(mousePosition, buttonType);
	}
}

void MouseEventDispatcher::NotifyMouseReleased(int buttonType)
{
	for (auto& listener : mouseEventListeners)
	{
		listener->OnMouseReleased(mousePosition, buttonType);
	}
}

void MouseEventDispatcher::NotifyMouseMove()
{
	for (auto& listener : mouseEventListeners)
	{
		listener->OnMouseMove(mousePosition);
	}
}

void MouseEventDispatcher::NotifyMouseScroll(int y)
{
	for (auto& listener : mouseEventListeners)
	{
		listener->OnMouseScroll(y);
	}
}

Vec2 MouseEventDispatcher::GetPreviousMousePosition()
{
	return prevPosition;
}

Vec2 MouseEventDispatcher::GetMousePosition()
{
	return mousePosition;
}

Vec2 MouseEventDispatcher::GetDeltaMousePosition()
{
	return mousePosition - prevPosition;
}

void MouseEventDispatcher::UpdateMousePosition()
{
	int x, y;

	SDL_GetMouseState(&x, &y);

	y = Globals::Engine::SCREEN_HEIGHT - y;

	if(firstUpdate)
	{
		prevPosition.x = mousePosition.x = static_cast<float>(x);	
		prevPosition.y = mousePosition.y = static_cast<float>(y);
		firstUpdate = false;
	}
	else
	{
		prevPosition = mousePosition;
		mousePosition = Vec2(static_cast<float>(x), static_cast<float>(y));
	}
}

