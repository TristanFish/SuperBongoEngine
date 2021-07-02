#include "MouseEventListener.h"
#include "core/events/MouseEventDispatcher.h"


MouseEventListener::MouseEventListener()
{
	MouseEventDispatcher::RegisterListener(this);
}

MouseEventListener::~MouseEventListener()
{
	MouseEventDispatcher::UnRegisterListener(this);
}
