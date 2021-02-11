#ifndef GAME1_H
#define GAME1_H

#include "core/CoreEngine.h"
#include "scenes/Scene1.h"
#include "scenes/Scene2.h"

class Game1 : public GameInterface
{
private:
	int currentSceneNum;
	Scene* currentScene;

	void BuildScene();

public:

	Game1();
	virtual ~Game1();

	// Inherited via GameInterface
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() override;
	virtual void HandleEvents(const SDL_Event& event) override;

};


#endif