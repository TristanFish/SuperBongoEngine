#ifndef AUDIOSOURCECOMPONENT_H
#define AUDIOSOURCECOMPONENT_H

#include "components/ECS.h"
#include "audio/AudioManager.h"
#include "audio/SoundAttributes.h"

class AudioSourceComponent : public Component
{
	public:
	public:
	void Init(GameObject* g) override;
	void Update(const float deltaTime) override;
	void Render() const override {}
	void HandleEvents(const SDL_Event& event) override {}

	/*! Retrieves a sound from the audio managers sound library and finds a viable channel to play sound */
	void PlaySound(FMOD::Sound* sound, FMOD::Channel** channelRef);

	
	FMOD::Sound* LoadSound(std::string soundName);
	/*! Returns a sound object to be stored on specific objects, faster than searching */
	FMOD::Sound* GetSound(std::string soundNameFromMap);
	
	~AudioSourceComponent();
	
private:
	/*! Gets the audio source's position in world space and converts to an FMOD Vector */
	void SetAudioPosition();
	
	SoundAttributes soundAtts;
	
	FMOD::System* audioSystemRef;

	FMOD_VECTOR FMODGamePos;
	MATH::Vec3 gameObjectPosition;
};
#endif;
