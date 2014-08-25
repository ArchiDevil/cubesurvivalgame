#include "game.h"

cGame::cGame()
{
	World				= new cWorld;
	Player				= new cPlayer;
	environmentMgr		= new cEnvironmentManager;
	EntityMgr			= new EntityManager;
	GameEventHandler	= new cGameEventHandler;
	gameHud				= new gameHUD;
	//ItemManager			= new cItemManager;
}

cGame::~cGame()
{
	delete ItemMgr;
	delete GameEventHandler;
	delete World;
	delete Player;
	delete EntityMgr;
	delete gameHud;
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
