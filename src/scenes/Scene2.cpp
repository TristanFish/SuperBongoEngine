#include "Scene2.h"
#include "core/CoreEngine.h"

bool Scene2::OnCreate()
{
	EngineLogger::Info("Scene 2 Created", "Scene2.cpp", __LINE__);

	objectList = new Manager();
	objectList->Init();

	return true;
}

void Scene2::Reset()
{
}

void Scene2::Update(float deltaTime_)
{
	Scene::Update(deltaTime_);
}

void Scene2::HandleEvents(const SDL_Event& event_)
{
	Scene::HandleEvents(event_);

	switch (event_.key.keysym.scancode)
	{
	case SDL_SCANCODE_F1:
		CoreEngine::GetInstance()->currentSceneNum = 0;
		break;
	case SDL_SCANCODE_F2:
		CoreEngine::GetInstance()->currentSceneNum = 1;
		break;
	}
}

void Scene2::Render() const
{
	Scene::Render();
	objectList->Render();
}
