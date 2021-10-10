#include "AIComponent.h"

#include "components/GameObject.h"

AIComponent::AIComponent()	{
	maxSpeed = 1.0f;
	maxAcceleration = 1.0f;
}

void AIComponent::Init(GameObject* g) {
	gameobject = g;
}

void AIComponent::Update(const float deltaTime)	{
	//rotate object

	//kinematic update
	gameobject->transform.rotation += Ksteering.rotation * deltaTime;

	if(gameobject->HasComponent<RigidBody3D>())	{
		if(VMath::mag(Ksteering.velocity) > maxSpeed)	{
			Ksteering.velocity = VMath::normalize(Ksteering.velocity)* maxSpeed;
		}
		gameobject->GetComponent<RigidBody3D>()->SetVelocity(Ksteering.velocity);
	}
	
	//needs dynamic update
}

void AIComponent::OnSaveComponent(const std::string& saveName, std::string parentName)	{
	
}

void AIComponent::HandleEvents(const SDL_Event& event)
{
}



