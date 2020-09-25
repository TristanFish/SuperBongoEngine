#ifndef AUDIOSOURCECOMPONENT_H
#define AUDIOSOURCECOMPONENT_H

#include "components/ECS.h"
#include "audio/AudioManager.h"
#include "SoundAttributes.h"
#include <cstdarg>

class AudioSourceComponent : public Component
{
	void Init(GameObject* g);
	void Update(const float deltaTime);
	void Render();
	void HandleEvents(const SDL_Event& event);

	void LoadSounds();
	void SetAudioPosition();
	

	~AudioSourceComponent() { gameobject = nullptr; gameObjectPosition = nullptr; }
	
private:
	SoundAttributes soundAtts;
	MATH::Vec3* gameObjectPosition;
	FMOD::System* audioSystemRef;

	std::vector <FMOD::Sound> sounds;
	std::vector <const char*> soundPaths;

};
#endif;
