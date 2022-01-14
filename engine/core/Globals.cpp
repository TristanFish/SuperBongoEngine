#include "Globals.h"

#include "components/SceneGraph.h"
#include "core/CoreEngine.h"
#include "core/scene/Scene.h"


std::string Globals::ENGINE_PATH = std::filesystem::current_path().string();
std::string Globals::SAVE_DATA_PATH = ENGINE_PATH + "\\resources\\SaveData\\";
std::string Globals::SCENE_NAME = "";



std::shared_ptr<SceneGraph> Globals::s_SceneGraph = nullptr;



std::string Globals::IntToVectorWithPrefix(const char* typePrefix, const int i)
{
	
		std::string Prefix = std::string(typePrefix) + "_";
		switch (i)
		{
		case 0:
			return  Prefix + "X";
			break;

		case 1:
			return Prefix + "Y";
			break;
		case 2:
			return Prefix + "Z";
			break;
		case 3:
			return Prefix + "W";
			break;
		}

	return "ERROR";
}


std::string Globals::IntToVector(const int i)
{
	switch (i)
	{
	case 0:
		return  "X";
		break;

	case 1:
		return "Y";
		break;
	case 2:
		return "Z";
		break;
	case 3:
		return "W";
		break;
	}
}

void Globals::InitGlobals()
{
	s_SceneGraph = CoreEngine::GetInstance()->GetCurrentScene()->objectList;

	SCENE_NAME = CoreEngine::GetInstance()->GetCurrentScene()->GetSceneName();
}

