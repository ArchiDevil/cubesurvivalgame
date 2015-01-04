#include "game.h"

cGame::cGame()
	: Player(nullptr)
{
	World.reset(new cWorld);
	environmentMgr.reset(new EnvironmentManager);
	EntityMgr.reset(new GameObjectsManager);
	GlobalEventHandler.reset(new GameEventHandler);
	gameHud.reset(new gameHUD);
}

cGame::~cGame()
{
}

static cGame * GamePtr = nullptr;

void LostIsland::CreateGame()
{
	delete GamePtr;
	GamePtr = new cGame();
}

cGame * LostIsland::GetGamePtr()
{
	return GamePtr;
}
