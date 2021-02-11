#ifndef COREENGINE_H
#define COREENGINE_H
#include "graphics/Window.h"
#include "Timer.h"
#include "sdl/SDL.h"
#include "scenes/Scene.h"
#include <Windows.h>
#include "scenes/Scene1.h"
#include "core/Logger.h"
#include "core/GameInterface.h"

class CoreEngine
{

private:
	Window* window;

	unsigned int fps;
	bool isRunning;

	static std::unique_ptr<CoreEngine> engineInstance;
	friend std::default_delete<CoreEngine>;

	GameInterface* gameInterface;


public:
	int currentSceneNum;
	static CoreEngine* GetInstance();

	CoreEngine(const CoreEngine&) = delete;
	CoreEngine(CoreEngine&&) = delete;
	CoreEngine& operator=(const CoreEngine&) = delete;
	CoreEngine& operator=(CoreEngine&&) = delete;

	bool Init();
	void Run();
	bool GetIsRunning();

	void SetGameInterface(GameInterface* gameInterface_);

	int GetCurrentScene() const;
	void ReloadCurrentScene();
	void OnDestroy();

private:
	CoreEngine();
	~CoreEngine();
	void Update(const float deltaTime_);
	void Render();
	void HandleEvents();



};


#endif