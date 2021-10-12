#ifndef AICOMPONENT_H
#define AICOMPONENT_H


#include "components/Component.h"
#include "components/AI/Algorithms/Kinematic.h"
#include "components/AI/Algorithms/Dynamic.h"

enum class AIType {
	KinematicSteering, 
	DynamicSteering
};

class AIComponent :	public Component	{
private:
	float maxSpeed;
	float maxAcceleration;
	Kinematic::KinematicSteeringOutput* kSteering;
	Dynamic::DynamicSteeringOutput* dSteering;

	//AIComponent is of type KinematicSteering by default
	AIType aiType;

public:
	AIComponent();
	~AIComponent();
	void Init(GameObject* g) override;
	void Update(const float deltaTime) override;
	void OnSaveComponent(const std::string& saveName, std::string parentName) override;
	void HandleEvents(const SDL_Event& event) override;
	
	void SetAIType(AIType aiType_);
	void SetSteering(SteeringOutput* steering_);

	float GetMaxSpeed() const { return maxSpeed; }
	float GetMaxAcceleration() const { return maxAcceleration; }

	//NOTE: have it check for a rigid body in the game object, since those are optional
};

#endif
