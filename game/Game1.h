#ifndef GAME1_H
#define GAME1_H

#include "core/GameInterface.h"

class Game1 final : public Game
{
private:
	int currentSceneNum;


public:

	Game1();
	~Game1();

	// Inherited via Game
	bool OnCreate() override;
};


#endif