#include "AiGameObject.h"

AiGameObject::AiGameObject(const std::string& name_, MATH::Vec3 position_)	{
	name = name_;
	SetPos(position_);
	
	mRenderer = AddComponent<MeshRenderer>();
	mRenderer->Init(this);
	mRenderer->LoadModel("Plane.fbx");
	mRenderer->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");

	bodyComponent = AddComponent<RigidBody3D>();
	bodyComponent->Init(this);
	bodyComponent->ConstructCollider(ColliderType::OBB);
	

	aiComponent = AddComponent<AIComponent>();
	aiComponent->SetAIType(AIType::KinematicSteering);
	aiComponent->SetMaxSpeed(5.0f);
	aiComponent->SetMaxAcceleration(5.0f);
	componentList;

	aiTarget = nullptr;

}

AiGameObject::~AiGameObject()	{

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
