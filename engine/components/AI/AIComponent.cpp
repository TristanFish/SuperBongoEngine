#include "AIComponent.h"

#include "components/GameObject.h"

AIComponent::AIComponent()	{
	maxSpeed = 1.0f;
	maxAcceleration = 1.0f;
}

void AIComponent::Init(GameObject* g) {
	
}

void AIComponent::Update(const float deltaTime)	{
	//rotate object
	gameobject->transform.rotation += steering.rotation * deltaTime;

	if(gameobject->HasComponent<RigidBody3D>())	{
		if(VMath::mag(steering.velocity) > maxSpeed)	{
			steering.velocity = VMath::normalize(steering.velocity)* maxSpeed;
		}
		gameobject->GetComponent<RigidBody3D>()->SetVelocity(steering.velocity);
	}
	
}

void AIComponent::OnSaveComponent(const std::string& saveName, std::string parentName)	{
	
}

void AIComponent::HandleEvents(const SDL_Event& event)
{
}



