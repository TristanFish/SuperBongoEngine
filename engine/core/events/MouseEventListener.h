#ifndef MOUSEEVENTLISTENER_H
#define MOUSEEVENTLISTENER_H

#include "math/Vector.h"

class MouseEventListener
{
public:
	MouseEventListener();
	
	virtual ~MouseEventListener();


	virtual void OnMousePressed(MATH::Vec2 mouse, int buttonType) {}
	virtual void OnMouseReleased(MATH::Vec2 mouse, int buttonType) {}
	virtual void OnMouseMove(MATH::Vec2 mouse) {}
	virtual void OnMouseScroll(int y) {}

private:
};

#endif