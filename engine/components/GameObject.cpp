#include "GameObject.h"
#include "Component.h"
#include "3D/MeshRenderer.h"

#include <functional>
#include "3D/RigidBody3D.h"

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
		if(MeshRenderer* mComp = dynamic_cast<MeshRenderer*>(comp))
		{
			mComp->AddUniformFunction(std::bind(&GameObject::AttachUniforms, this));
		}
	}
}

void GameObject::PostInit()
{
	for (Component* comp : componentList)
	{
		if (RigidBody3D* rbComp = dynamic_cast<RigidBody3D*>(comp))
		{
			rbComp->GetCollider()->AddCollisionFunction(std::bind(&GameObject::OnCollisionEnter, this, std::placeholders::_1));
		}
	}
}

void GameObject::Update(const float deltaTime)	{
	transform.Update(deltaTime);
	for(Component* comp : componentList)	{
		if(comp->active)	{

			if (dynamic_cast<RigidBody3D*>(comp) == nullptr)
			{
				comp->Update(deltaTime);
			}

		}
	}
}

void GameObject::UpdateTransform()
{
	transform.Update(0.0f);
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