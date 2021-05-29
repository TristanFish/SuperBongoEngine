#ifndef MOUSEEVENTDISPATCHER_H
#define MOUSEEVENTDISPATCHER_H

#include "math/Vector.h"
#include "MouseEventListener.h"
#include <vector>
#include <sdl/SDL.h>

class MouseEventDispatcher
{
public:
	MouseEventDispatcher(const MouseEventDispatcher&) = delete;
	MouseEventDispatcher(MouseEventDispatcher&&) = delete;
	MouseEventDispatcher& operator=(const MouseEventDispatcher&) = delete;
	MouseEventDispatcher& operator=(MouseEventDispatcher&&) = delete;

	MouseEventDispatcher() = delete;
	~MouseEventDispatcher();

	static void RegisterListener(MouseEventListener* listener);
	static void UnRegisterListener(MouseEventListener* listener);
	static void Update(SDL_Event e);

	static void NotifyMousePressed(int buttonType);
	static void NotifyMouseReleased(int buttonType);
	static void NotifyMouseMove();
	static void NotifyMouseScroll(int y);

	static MATH::Vec2 GetPreviousMousePosition();
	static MATH::Vec2 GetMousePosition();
	static MATH::Vec2 GetDeltaMousePosition();

private:

	static std::vector<MouseEventListener*> mouseEventListeners;
	static MATH::Vec2 mousePosition, prevPosition;
	static bool firstUpdate;
	static void UpdateMousePosition();
	
};

#endif