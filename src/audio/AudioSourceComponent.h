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

	void PlaySound(FMOD::Sound* sound);
	FMOD::Sound* GetFreshSoundBuffer();
	FMOD::Sound* LoadSound(const char* soundName, FMOD::Sound* soundBuffer);
	FMOD::Sound* GetSound(int soundNumber);
	
	~AudioSourceComponent();
	
private:

	void SetAudioPosition();
	void LoadSoundVector();

	std::vector <FMOD::Sound*> sounds;
	FMOD::Sound* sound1;
	FMOD::Sound* sound2;
	FMOD::Sound* sound3;
	SoundAttributes soundAtts;
	
	FMOD::System* audioSystemRef;
	FMOD_VECTOR FMODGamePos;
	MATH::Vec3 gameObjectPosition;
};
#endif;
