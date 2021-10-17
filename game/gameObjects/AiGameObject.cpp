#include "AiGameObject.h"

AiGameObject::AiGameObject(std::string name_, MATH::Vec3 position_)	{
	name = name_;
	SetPos(position_);
	mRenderer = AddComponent<MeshRenderer>();
	mRenderer->LoadModel("Cube.fbx");
	mRenderer->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");

	AddComponent<RigidBody3D>();

	aiComponent = AddComponent<AIComponent>();
	aiComponent->SetAIType(AIType::KinematicSteering);
	aiComponent->SetMaxSpeed(7.0f);
	aiComponent->SetMaxAcceleration(2.5f);

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
		Kinematic::KinematicArrive kSeekAlgorithm = Kinematic::KinematicArrive(this, aiTarget->transform, 5.0f, 1.0f);
		//steering is already being set by the algorithm
		kSeekAlgorithm.getSteering();
	}

	this->GameObject::Update(deltaTime);
}
