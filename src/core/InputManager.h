#pragma once

#include "sdl/SDL.h"
#include <unordered_map>

class InputManager
{
public:
	static InputManager* GetInstance();
	static void RemoveInstance();

	void PollEvents(SDL_Event& event);

	bool GetKey(SDL_Keycode key);
	bool GetMouseButton(Uint8 mouseButton);

private:
	static InputManager* instance;
	std::unordered_map<SDL_Keycode, bool> keys;
	std::unordered_map<Uint8, bool> mouseButtons;
};


