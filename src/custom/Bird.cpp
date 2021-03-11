#include "Bird.h"

Bird::Bird(const char* name, MATH::Vec3 position) {

	AddComponent<MeshRenderer>()->LoadModel("Grass.fbx");
	GetComponent<MeshRenderer>()->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");

	birdSound = AudioManager::Get()->RetrieveSoundObject("birdchirp");
	AddComponent<AudioSourceComponent>()->PlaySound(birdSound, &AudioManager::Get()->C1);
	
	this->name = name;
	this->transform.pos = position;

	this->transform.SetRot(MATH::Vec3(-90.0f, 0.0f, 90.0f));
	this->transform.scale = MATH::Vec3(0.1f, 0.1f, 0.1f);
	this->transform.SetPos(MATH::Vec3(0.0f, 6.0f, 0.0f));

}

const char* Bird::GetClassIDName() const
{
	return "Bird";
}

