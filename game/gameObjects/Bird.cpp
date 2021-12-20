#include "Bird.h"
#include "components/Components.h"

Bird::Bird(const std::string& name, MATH::Vec3 position) {

	AddComponent<MeshRenderer>()->LoadModel("PuffinBall.fbx");
	GetComponent<MeshRenderer>()->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");

	birdSound = AudioManager::Get()->RetrieveSoundObject("birdchirp");
	AddComponent<AudioSourceComponent>()->PlaySound(birdSound, &AudioManager::Get()->C1);
	
	this->name = name;
	this->transform.SetPos(position);

	this->transform.SetRot(MATH::Vec3(0.0f, 0.0f, 90.0f));
	this->transform.SetScale(MATH::Vec3(0.1f, 0.1f, 0.1f));
	this->transform.SetPos(MATH::Vec3(0.0f, 6.0f, 20.0f));

}
