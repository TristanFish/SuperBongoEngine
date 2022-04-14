#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include "core/CoreEngine.h"
#include "rendering/Camera.h"

class CameraManager
{
	CameraManager();

	static std::unique_ptr<CameraManager> ManagerInstance;
	friend std::default_delete<CameraManager>;

	std::unique_ptr<Camera> EditorCamera;
	std::unique_ptr<Camera> GameplayCamera;

public:
	static CameraManager* GetInstance();
	Camera* GetCamera();
	Camera* GetCamera(CameraTypes type);

};

#endif