#ifndef COREENGINE_H
#define COREENGINE_H
#include <memory>


class Window;
class GameInterface;
class Scene;
class CoreEngine
{

private:
	Window* window;

	unsigned int fps;
	bool isRunning;

	static std::unique_ptr<CoreEngine> engineInstance;
	friend std::default_delete<CoreEngine>;


public:
	int currentSceneNum;
	GameInterface* gameInterface;
	
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