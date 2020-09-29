#ifndef AUDIOLISTENERCOMPONENT_H
#define AUDIOLISTENERCOMPONENT_H

#include "components/ECS.h"
#include "audio/AudioManager.h"
#include <fmod/fmod.h>
#include <fmod/fmod.hpp>


class AudioListenerComponent : public Component
{
private:
	FMOD::System* audioSystemRef;
	FMOD_VECTOR* FMODlistenerPos;
	FMOD_VECTOR* FMODForward;
	FMOD_VECTOR FMODUp;
	MATH::Vec3* listenerPos;

public:

	void Init(GameObject* g);
	void Update(const float deltaTime);
	void Render();
	void HandleEvents(const SDL_Event& event);

	void SetListenerPosition();
};
#endif;
