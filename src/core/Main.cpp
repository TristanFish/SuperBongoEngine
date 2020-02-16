#include "GameManager.h"

int main(int argc, char* argv[])
{
	GameManager* game = new GameManager();

	game->Init();
	game->Run();

	delete game;
	game = nullptr;

	return 0;
}