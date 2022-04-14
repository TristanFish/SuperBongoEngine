#include "CameraManager.h"

std::unique_ptr<CameraManager> CameraManager::ManagerInstance = std::unique_ptr<CameraManager>();

CameraManager::CameraManager()
{
	EditorCamera = std::make_unique<Camera>();
	EditorCamera->SetCameraType(CameraTypes::Editor);
	GameplayCamera = std::make_unique<Camera>();
	GameplayCamera->SetCameraType(CameraTypes::Gameplay);
}

CameraManager* CameraManager::GetInstance()
{
	if (ManagerInstance == nullptr)
	{
		ManagerInstance.reset(new CameraManager);
	}
	return ManagerInstance.get();
}

Camera* CameraManager::GetCamera()
{
	if(CoreEngine::GetInstance()->GetIsGameRunning())
	{
		return GameplayCamera.get();
	} 
	else
	{
		return EditorCamera.get();
	}
}

Camera* CameraManager::GetCamera(CameraTypes type)
{
	switch (type)
	{
	case CameraTypes::Editor:
		return EditorCamera.get();
		break;
	case CameraTypes::Gameplay:
		return GameplayCamera.get();
		break;
	default:
		break;
	}

	return nullptr;
}