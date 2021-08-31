#include "MouseEventListener.h"
#include "MouseEventDispatcher.h"


MouseEventListener::MouseEventListener()
{
	MouseEventDispatcher::RegisterListener(this);
}

MouseEventListener::~MouseEventListener()
{
	MouseEventDispatcher::UnRegisterListener(this);
}
