#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>
#include "core/CoreEngine.h"
#include "Games/Game1.h"
#include "core/GameInterface.h"

int main(int argc, char* argv[])
{	

	CoreEngine::GetInstance()->SetGameInterface(new Game1());
	if(!CoreEngine::GetInstance()->Init())
	{
		printf("Engine failed to Launch");
		return 0;
	}
	CoreEngine::GetInstance()->Run();
	
	
	_CrtDumpMemoryLeaks();

	return 0;
}