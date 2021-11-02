#ifndef COREENGINE_H
#define COREENGINE_H
#include <memory>


namespace CustomUI {
	class DockSpace;
}

class Window;
class GameInterface;
class Scene;
class CoreEngine
{

private:
	Window* window;

	bool isRunning;

	static std::unique_ptr<CoreEngine> engineInstance;
	friend std::default_delete<CoreEngine>;


public:
	unsigned int fps;
	bool limitfps = true;
	int currentSceneNum;
	GameInterface* gameInterface;
	CustomUI::DockSpace* dockSpace;
	
	static CoreEngine* GetInstance();

	CoreEngine(const CoreEngine&) = delete;
	CoreEngine(CoreEngine&&) = delete;
	CoreEngine& operator=(const CoreEngine&) = delete;
	CoreEngine& operator=(CoreEngine&&) = delete;

	bool Init();
	void Run();
	bool GetIsRunning() const;

	void SetGameInterface(GameInterface* gameInterface_);

	int GetCurrentSceneNum() const;
	Scene* GetCurrentScene() const;

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