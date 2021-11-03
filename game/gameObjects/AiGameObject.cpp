#include "AiGameObject.h"

AiGameObject::AiGameObject(std::string name_, MATH::Vec3 position_)	{
	name = name_;
	SetPos(position_);
	mRenderer = AddComponent<MeshRenderer>();
	mRenderer->LoadModel("Plane.fbx");
	mRenderer->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");

	AddComponent<RigidBody3D>();

	aiComponent = AddComponent<AIComponent>();
	aiComponent->SetAIType(AIType::KinematicSteering);
	aiComponent->SetMaxSpeed(5.0f);
	aiComponent->SetMaxAcceleration(5.0f);

	aiTarget = nullptr;

}

AiGameObject::~AiGameObject()	{
	if(mRenderer)	{
		delete mRenderer;
		mRenderer = nullptr;
	}
	if(aiComponent)	{
		delete aiComponent;
		aiComponent = nullptr;
	}
}

void AiGameObject::Update(const float deltaTime)	{
	if(aiTarget)	{
		Kinematic::KinematicSeek kSeekAlgo = Kinematic::KinematicSeek(this, aiTarget->transform);
		Kinematic::KinematicSteeringOutput steering = kSeekAlgo.getSteering();
		aiComponent->SetSteering(&steering);
		
		/*Kinematic::KinematicArrive kArriveAlgo = Kinematic::KinematicArrive(this, aiTarget->transform, 10.0f, 1.0f);
		Kinematic::KinematicSteeringOutput steering = kSeekAlgo.getSteering();
		aiComponent->SetSteering(&steering);*/

		/*Dynamic::DynamicSeek dSeekAlgo = Dynamic::DynamicSeek(this, aiTarget->transform);
		dSeekAlgo.getSteering();*/

		/*Dynamic::DynamicArrive dArriveAlgo = Dynamic::DynamicArrive(this, aiTarget->transform, 10.0f, 25.0f, 1.0f);
		dArriveAlgo.getSteering();*/
		
		/*Dynamic::DynamicFlee dFleeAlgo = Dynamic::DynamicFlee(this, aiTarget->transform);
		dFleeAlgo.getSteering();*/

		
		
	}

	this->GameObject::Update(deltaTime);
}
