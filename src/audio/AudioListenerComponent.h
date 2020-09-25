#ifndef AUDIOLISTENERCOMPONENT_H
#define AUDIOLISTENERCOMPONENT_H

#include "components/ECS.h"

class AudioListenerComponent : public Component
{
	void Init(GameObject* g);
	void Update(const float deltaTime);
	void Render();
	void HandleEvents(const SDL_Event& event);
};
#endif;
