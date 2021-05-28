#include "MouseEventListener.h"
#include "core/MouseEventDispatcher.h"


MouseEventListener::MouseEventListener()
{
	MouseEventDispatcher::RegisterListener(this);
}

MouseEventListener::~MouseEventListener()
{
	MouseEventDispatcher::UnRegisterListener(this);
}
