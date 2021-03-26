#ifndef GAME1_H
#define GAME1_H

#include "scenes/Scene.h"
#include "core/GameInterface.h"

class Game1 final : public GameInterface
{
private:
	int currentSceneNum;
	Scene* currentScene;

	void BuildScene();

public:

	Game1();
	~Game1();

	// Inherited via GameInterface
	bool OnCreate() override;
	void OnDestroy() override;
	void Update(const float deltaTime_) override;
	void Render() override;
	void HandleEvents(const SDL_Event& event) override;

};


#endif