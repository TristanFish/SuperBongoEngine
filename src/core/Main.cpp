#include "GameManager.h"
#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>
#include "graphics/Window.h"

int main(int argc, char* argv[])
{
	GameManager* game = new GameManager();

	game->Init();
	game->Run();

	delete game;
	game = nullptr;

	_CrtDumpMemoryLeaks();

	return 0;
}