#include "AiGameObject.h"
#include "core/events/InputManager.h"

AiGameObject::AiGameObject(const std::string& name_, MATH::Vec3 position_)	{
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


	/*Decision tree will look like
	              is searching?
				yes	/		\ no
				   /		 \
   print: searching		is y position in range?
					yes	/					 \ no
					   /					  \
		print: y pos in range				 is integer in range?
										yes	/					\ no
										   /					 \
					print: integer is in range				 is string equal to Hello?
														yes	/						 \ no
														   /                          \
										print: Type in Hello						print: Hello was not typed in


	*/
	BoolDecision* boolDecision = new BoolDecision(&isSearching);
	PrintAction* bAction = new PrintAction("Searching");
	boolDecision->AddTrueNode(bAction);

	FloatDecision* floatDecision = new FloatDecision(0.0f, 10.0f, &transform.GetPositionRef().y);
	PrintAction* fAction = new PrintAction("Gameobject's y position is in range");
	boolDecision->AddFalseNode(floatDecision);
	floatDecision->AddTrueNode(fAction);

	IntDecision* intDecision = new IntDecision(5, 10, &range);
	PrintAction* iAction = new PrintAction("Integer is in range");
	floatDecision->AddFalseNode(intDecision);
	intDecision->AddTrueNode(iAction);

	StringDecision* sDecision = new StringDecision("Hello", &input);
	PrintAction* sActionTrue = new PrintAction("Typed In Hello");
	PrintAction* sActionFalse = new PrintAction("Hello was not typed in");
	intDecision->AddFalseNode(sDecision);
	sDecision->AddTrueNode(sActionTrue);
	sDecision->AddFalseNode(sActionFalse);

	dTree = new DecisionTree();
	dTree->d = boolDecision;
}

AiGameObject::~AiGameObject()	
{
	if(dTree)
	{
		delete dTree;
	}
}

void AiGameObject::Update(const float deltaTime)	{

	if(InputManager::GetInstance()->GetKey(SDLK_y))
	{
		dTree->EvaluateDecisionTree();
	}


	//if(aiTarget)	{
	//	Kinematic::KinematicSeek kSeekAlgo = Kinematic::KinematicSeek(this, aiTarget->transform);
	//	Kinematic::KinematicSteeringOutput steering = kSeekAlgo.getSteering();
	//	aiComponent->SetSteering(&steering);
	//	
	//	/*Kinematic::KinematicArrive kArriveAlgo = Kinematic::KinematicArrive(this, aiTarget->transform, 10.0f, 1.0f);
	//	Kinematic::KinematicSteeringOutput steering = kSeekAlgo.getSteering();
	//	aiComponent->SetSteering(&steering);*/

	//	/*Dynamic::DynamicSeek dSeekAlgo = Dynamic::DynamicSeek(this, aiTarget->transform);
	//	dSeekAlgo.getSteering();*/

	//	/*Dynamic::DynamicArrive dArriveAlgo = Dynamic::DynamicArrive(this, aiTarget->transform, 10.0f, 25.0f, 1.0f);
	//	dArriveAlgo.getSteering();*/
	//	
	//	/*Dynamic::DynamicFlee dFleeAlgo = Dynamic::DynamicFlee(this, aiTarget->transform);
	//	dFleeAlgo.getSteering();*/

	//	
	//	
	//}

	this->GameObject::Update(deltaTime);
}

void AiGameObject::ImguiRender()
{
	ImGui::Checkbox("Searching", &isSearching);
	ImGui::InputText("Input String", &input);
	ImGui::SliderInt("Integer value", &range, -30, 30);
}

