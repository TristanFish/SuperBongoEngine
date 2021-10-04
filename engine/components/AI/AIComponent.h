#ifndef AICOMPONENT_H
#define AICOMPONENT_H


#include "components/Component.h"
#include "components/AI/Algorithms/Kinematic.h"

class AIComponent :	public Component	{
private:
	float maxSpeed;
	float maxAcceleration;
	Kinematic::KinematicSteeringOutput steering;

public:
	AIComponent();
	void Init(GameObject* g) override; //fix this
	void Update(const float deltaTime) override;
	void OnSaveComponent(const std::string& saveName, std::string parentName) override;
	void HandleEvents(const SDL_Event& event) override;
	
	void SetSteering(Kinematic::KinematicSteeringOutput steering_) { steering = steering_; }
	
	float GetMaxSpeed() const { return maxSpeed; }
	float GetMaxAcceleration() const { return maxAcceleration; }

	//NOTE: have it check for a rigid body in the game object, since those are optional
};

#endif
