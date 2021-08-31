#include "Globals.h"

std::string Globals::ENGINE_PATH = std::filesystem::current_path().string();

std::string Globals::IntToVec3(const int i)
{
	switch (i)
	{
	case 0:
		return "X";
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

	return "ERROR";
}

//int Globals::SCREEN_HEIGHT;
//int Globals::SCREEN_WIDTH;