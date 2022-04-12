#ifndef AIGAMEOBJECT_H
#define AIGAMEOBJECT_H

#include "components/ECS.h"

class AIComponent;
class DecisionTree;
class AiGameObject : public GameObject	{
public:
	MeshRenderer* mRenderer;
	AIComponent* aiComponent;
	RigidBody3D* bodyComponent;
	
	std::shared_ptr<GameObject> aiTarget;

	DecisionTree* dTree;

	bool isSearching;
	int range;
	std::string input; 

	AiGameObject(const std::string& name_, MATH::Vec3 position_);
	~AiGameObject() override;

	void Update(const float deltaTime) override;

	void ImguiRender() override;

	void PostInit() override;
	
	//AiGameObject* NewClone() const override { return new AiGameObject(this->name, this->transform.GetPosition()); };
	std::shared_ptr<GameObject> NewClone() const override { return std::make_shared<AiGameObject>(this->name, this->transform.GetPosition()); }

};

#endif
