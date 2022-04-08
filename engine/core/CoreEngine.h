#ifndef COREENGINE_H
#define COREENGINE_H
#include <memory>


namespace CustomUI {
	class DockSpace;
}

class Window;
class Game;
class Scene;
class CoreEngine
{

private:
	Window* window;

	bool isEngineRunning;
	bool isGameRunning;

	static std::unique_ptr<CoreEngine> engineInstance;
	friend std::default_delete<CoreEngine>;


public:
	unsigned int fps;
	bool limitfps = true;
	int currentSceneNum;
	Game* gameInterface;
	CustomUI::DockSpace* dockSpace;
	
	static CoreEngine* GetInstance();

	CoreEngine(const CoreEngine&) = delete;
	CoreEngine(CoreEngine&&) = delete;
	CoreEngine& operator=(const CoreEngine&) = delete;
	CoreEngine& operator=(CoreEngine&&) = delete;

	bool Init();
	void Run();

	void SetGameInterface(Game* gameInterface_);
	bool GetIsGameRunning() const { return isGameRunning; }
	void SetIsGameRunning(bool status) { isGameRunning = status; };

	int GetCurrentSceneNum() const;
	Scene* GetCurrentScene() const;

	void ReloadCurrentScene();
	void OnDestroy();
	
	void SaveSceneData() const;
	void LoadSceneData();

	void PlayScene();
	void StopScene();

private:
	CoreEngine();
	~CoreEngine();
	void Update(const float deltaTime_);
	void UpdatePhysics(const float deltaTime_);
	void Render();
	void HandleEvents();




};


#endif