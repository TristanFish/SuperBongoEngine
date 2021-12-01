#include "GameObject.h"
#include "Component.h"
#include "3D/MeshRenderer.h"

#include <functional>


GameObject::GameObject(): parent(nullptr), name("Default")
{
}

GameObject::~GameObject()
{

	for(Component* comp : componentList)
	{
		if(comp)
		{
			delete comp;
			comp = nullptr;
		}
	}
	componentList.clear();
}

void GameObject::Init()
{
	for (Component* comp : componentList)
	{
		comp->Init(this);
		if(RigidBody3D* rbComp = dynamic_cast<RigidBody3D*>(comp))
		{
			rbComp->GetCollider()->AddCollisionFunction(std::bind(&GameObject::OnCollisionEnter, this, std::placeholders::_1));
		}
		else if(MeshRenderer* mComp = dynamic_cast<MeshRenderer*>(comp))
		{
			mComp->AddUniformFunction(std::bind(&GameObject::AttachUniforms, this));
		}
	}
}

void GameObject::Update(const float deltaTime)	{
	transform.Update(deltaTime);
	for(Component* comp : componentList)	{
		if(comp->active)	{
			comp->Update(deltaTime);
		}
	}
}

void GameObject::HandleEvents(const SDL_Event& event)
{
	for(Component* comp : componentList)
	{
		if(comp->active)
		{
			comp->HandleEvents(event);
		}
	}
}

