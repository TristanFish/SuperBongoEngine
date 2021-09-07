#pragma once

#include <iostream>
#include <filesystem>


struct Globals
{
	static const int SCREEN_WIDTH = 1920;
	static const int SCREEN_HEIGHT = 1080;
	static std::string ENGINE_PATH;


	static std::string IntToVec3(const int i);
	
};