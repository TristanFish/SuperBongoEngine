#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H


#include "sdl/SDL.h"
#include "math/Vector.h"
#include <unordered_map>
#include <memory>



enum MouseButton : Uint8
{
	MBUTTON_NONE = 0,
	MBUTTON_LEFT = 1,
	MBUTTON_RIGHT = 2,
	MBUTTON_MIDDLE = 4
};

class MouseState
{
private:
	MouseState()
	{
		prevPosition = MATH::Vec2();
		position = MATH::Vec2();

		buttonsPressed = 0;

		MouseScroll = 0;
	}
	
	MATH::Vec2 prevPosition;
	MATH::Vec2 position;
	
	Uint8 buttonsPressed;

	int MouseScroll;

public:

	friend class InputManager;

	MATH::Vec2 GetPreviousPosition() const { return prevPosition; }
	MATH::Vec2 GetCurrentPosition() const { return position; }
	MATH::Vec2 GetDeltaPosition() const { return position - prevPosition; }
	
	bool GetButton(MouseButton buttonToCheck) const {return (buttonsPressed & buttonToCheck); }
	

	
};

//! InputManager Class
/*! Manages any user inputs in the engine */
class InputManager
{
public:
	//! Static GetInstance Pointer Function
	/*! Returns the instance of this InputManager */
	static InputManager* GetInstance();

	//! Static RemoveInstance Function
	/*! Removes the instance of the InputManager */
	static void RemoveInstance();

	//! PollEvents Function
	/*! Handles SDL Keyboard/Mouse Events */
	void PollEvents(SDL_Event& event);


	//! GetKey Getter
	/*! Returns true if the key is pressed */
	bool GetKey(SDL_Keycode key);

	//! GetMouseButton Getter
	/*! Returns true if the mouse button is pressed */
	bool GetMouseButton(Uint8 mouseButton) { return false; }

private:

	//! Instance of the InputManager
	/*! A instance of the InputManager class */
	//static InputManager* instance;
	static std::unique_ptr<InputManager> instance;

	//! Keys unordered_map
	/*! Stores all the keys that can be pressed */
	std::unordered_map<SDL_Keycode, bool> keys;

	MouseState mouse;
	
};


#endif