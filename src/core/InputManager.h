#pragma once

#include "sdl/SDL.h"
#include <unordered_map>


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
	bool GetMouseButton(Uint8 mouseButton);

private:

	//! Instance of the InputManager
	/*! A instance of the InputManager class */
	static InputManager* instance;

	//! Keys unordered_map
	/*! Stores all the keys that can be pressed */
	std::unordered_map<SDL_Keycode, bool> keys;

	//! mouseButtons unordered_map
	/*! Stores all the mouse buttons that can be pressed */
	std::unordered_map<Uint8, bool> mouseButtons;
};


