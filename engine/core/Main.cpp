#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>

#include "core/CoreEngine.h"
#include "Game1.h"
#include "core/GameInterface.h"


int main(int argc, char* argv[])
{	
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	
	
	CoreEngine::GetInstance()->SetGameInterface(new Game1);
	if(!CoreEngine::GetInstance()->Init())
	{
		printf("Engine failed to Launch");
		return 0;
	}
	CoreEngine::GetInstance()->Run();

	return 0;
}