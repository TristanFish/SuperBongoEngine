#pragma once

#include <iostream>
#include <filesystem>
#include <memory>
#include "components/SceneGraph.h"


class SceneGraph;


struct Globals
{
	static std::shared_ptr<SceneGraph> s_SceneGraph;


	static const int SCREEN_WIDTH = 1536;
	static const int SCREEN_HEIGHT = 864;
	
	static std::string SCENE_NAME;

	static std::string ENGINE_PATH;
	static std::string SAVE_DATA_PATH;



	static std::string IntToVectorWithPrefix(const char* typePrefix,const int i);

	static std::string IntToVector(const int i);
	
	static std::string FloatToPrefixFloat(const char* typePrefix, const float f);

	static SceneGraph* GetSceneGraph();

	static void InitGlobals();
	
};