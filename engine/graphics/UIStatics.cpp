#include "UIStatics.h"

#include "components/GameObject.h"
#include "components/SceneGraph.h"
#include "core/CoreEngine.h"
#include "scenes/Scene.h"

GameObject* UIStatics::selectedObj = nullptr;

std::shared_ptr<SceneGraph> UIStatics::sceneGraph = nullptr;



UIStatics::~UIStatics()
{

}

void UIStatics::ConstructUIStatics()
{
	sceneGraph = CoreEngine::GetInstance()->GetCurrentScene()->objectList;

}