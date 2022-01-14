#ifndef AIGAMEOBJECT_H
#define AIGAMEOBJECT_H

#include "components/ECS.h"
#include "components/AI/AIComponent.h"

class AiGameObject : public GameObject	{
public:
	MeshRenderer* mRenderer;
	AIComponent* aiComponent;
	
	GameObject* aiTarget;

	AiGameObject(const std::string& name_, MATH::Vec3 position_);
	~AiGameObject() override;

	void Update(const float deltaTime) override;
	
	AiGameObject* NewClone() const override { return new AiGameObject(this->name, this->transform.GetPosition()); };
};

#endif
