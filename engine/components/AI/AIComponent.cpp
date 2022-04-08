#include "AIComponent.h"

#include "components/GameObject.h"


AIComponent::AIComponent()	{
	maxSpeed = 1.0f;
	maxAcceleration = 1.0f;
	kSteering = Kinematic::KinematicSteeringOutput(Vec3(0.0f), Vec3(0.0f));
	dSteering = Dynamic::DynamicSteeringOutput(Vec3(0.0f), Vec3(0.0f));
	aiType = AIType::KinematicSteering;
}

AIComponent::~AIComponent()	{

}

void AIComponent::Init(std::shared_ptr<GameObject> g) {
	gameObject = g;
	//AIComponent();
	
}

void AIComponent::Update(const float deltaTime)	{
	switch (aiType)	{
	case AIType::KinematicSteering:
		kSteering.Update(deltaTime, gameObject);
		break;

	case AIType::DynamicSteering:
		dSteering.Update(deltaTime, gameObject);
		break;

	default:
		EngineLogger::Warning(gameObject->GetName() + " has an incorrect aiType. No steering has been updated ",
			"AIComponent.cpp", __LINE__);
		break;
	}
	

}

void AIComponent::OnSaveComponent(const std::string& saveName, std::string parentName)	{
	
}

void AIComponent::HandleEvents(const SDL_Event& event)
{
}

void AIComponent::SetAIType(AIType aiType_)	{
	aiType = aiType_;
}

void AIComponent::SetSteering(SteeringOutput* steering_)	{
	if (Kinematic::KinematicSteeringOutput * kSteer = dynamic_cast<Kinematic::KinematicSteeringOutput*>(steering_)) {
		kSteering = *kSteer;
	}
	else if (Dynamic::DynamicSteeringOutput * dSteer = dynamic_cast<Dynamic::DynamicSteeringOutput*>(steering_)) {
		dSteering = *dSteer;
	}
}



