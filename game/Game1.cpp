#include "Game1.h"
#include "core/CoreEngine.h"
#include "core/Globals.h"
#include "scenes/Scene1.h"
#include "scenes/Scene2.h"
#include "core/scene/DefaultScene.h"

#include "scenes/SceneAi.h"
#include "Utility/LoadUtility.h"


Game1::Game1() : currentSceneNum(0)
{
	currentScene = nullptr;
	Scenes = std::vector<Scene*>();

	currentScene = new Scene1(); 
	//currentScene = new SceneAi();

	Scenes.push_back(currentScene);
	Scenes.push_back(new Scene2);
	//Scenes.push_back(new SceneAi);
}

Game1::~Game1()
{
}

bool Game1::OnCreate()
{
	return BuildScene();
}