#ifndef AUDIOSOURCECOMPONENT_H
#define AUDIOSOURCECOMPONENT_H

#include "components/ECS.h"
#include "audio/AudioManager.h"
#include "SoundAttributes.h"
#include <vector>

class AudioSourceComponent : public Component
{
	public:
	void Init(GameObject* g);
	void Update(const float deltaTime);
	void Render();
	void HandleEvents(const SDL_Event& event);

	void PlaySound(FMOD::Sound* sound, FMOD::Channel** channelRef);

	FMOD::Sound* LoadSound(std::string soundName);
	FMOD::Sound* GetSound(std::string soundNameFromMap);
	
	~AudioSourceComponent();
	
private:

	void SetAudioPosition();
	
	SoundAttributes soundAtts;
	
	FMOD::System* audioSystemRef;

	FMOD_VECTOR FMODGamePos;
	MATH::Vec3 gameObjectPosition;
};
#endif;
