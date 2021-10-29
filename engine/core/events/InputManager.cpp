#include "InputManager.h"
std::unique_ptr<InputManager> InputManager::instance = nullptr;

using namespace MATH;

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

	int mousex;
	int mousey;
	
	Uint32 mouseButton = SDL_GetMouseState(&mousex, &mousey);

	mouse.prevPosition = mouse.position;
	mouse.position = Vec2(static_cast<float>(mousex), static_cast<float>(mousey));
	
}

bool InputManager::GetKey(SDL_Keycode key)
{

	return keys[key];


}
