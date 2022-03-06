#ifndef AUDIOLISTENERCOMPONENT_H
#define AUDIOLISTENERCOMPONENT_H

#include <fmod/fmod.h>
#include <fmod/fmod.hpp>

#include "audio/AudioManager.h"

#include "Component.h"

class GameObject;
class AudioListenerComponent : public Component
{
	//This class could probably be a singleton like the audio manager but in case we ever want to have any split screen support we wont do that
	//That being said there should still only be 1 listener at a time for now similar to Unity
private:
	FMOD::System* audioSystemRef;

	FMOD_VECTOR* FMODlistenerPos;
	FMOD_VECTOR* FMODForward;
	FMOD_VECTOR FMODUp;

	MATH::Vec3* listenerPos;

public:

	void Init(GameObject* g) override;
	void Update(const float deltaTime) override;
	void HandleEvents(const SDL_Event& event) override {}
	
	/*! Sets the listener position to the posiiton of the game object inheriting the listener component */
	void SetListenerPosition();
};
#endif