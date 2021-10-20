#include "AiGameObject.h"

AiGameObject::AiGameObject(std::string name_, MATH::Vec3 position_)	{
	name = name_;
	SetPos(position_);
	mRenderer = AddComponent<MeshRenderer>();
	mRenderer->LoadModel("Sphere.fbx");
	mRenderer->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");

	AddComponent<RigidBody3D>();

	aiComponent = AddComponent<AIComponent>();
	aiComponent->SetAIType(AIType::DynamicSteering);
	aiComponent->SetMaxSpeed(15.0f);
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
		//Kinematic::KinematicSeek kSeekAlgorithm = Kinematic::KinematicSeek(this, aiTarget->transform);
		////steering is already being set by the algorithm
		//kSeekAlgorithm.getSteering();

		Dynamic::DynamicFlee dynamicSeek = Dynamic::DynamicFlee(this, aiTarget->transform);
		dynamicSeek.getSteering();
	}

	this->GameObject::Update(deltaTime);
}
