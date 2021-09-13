#pragma once

#include <iostream>
#include <filesystem>
#include <memory>


class SceneGraph;
struct Globals
{
	static std::shared_ptr<SceneGraph> s_SceneGraph;


	static const int SCREEN_WIDTH = 1780;
	static const int SCREEN_HEIGHT = 1000;
	static std::string ENGINE_PATH;


	static std::string IntToVector(const int i);



	static void InitGlobals();
	
};