#include "InputManager.h"

std::unique_ptr<InputManager> InputManager::instance = nullptr;

InputManager* InputManager::GetInstance()
{
	if (instance.get() == nullptr)
	{
		instance.reset(new InputManager);
	}
	return instance.get();
}

void InputManager::RemoveInstance()
{
	instance->keys.clear();
}

void InputManager::PollEvents(SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN)
	{
		keys[event.key.keysym.sym] = true;
	}

	if (event.type == SDL_KEYUP)
	{
		keys[event.key.keysym.sym] = false;
	}

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		mouseButtons[event.button.button] = true;
	}

	if (event.type == SDL_MOUSEBUTTONUP)
	{
		mouseButtons[event.button.button] = false;
	}
}

bool InputManager::GetKey(SDL_Keycode key)
{
	return keys[key];
}

bool InputManager::GetMouseButton(Uint8 mouseButton)
{
	return mouseButtons[mouseButton];
}
