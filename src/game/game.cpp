#include "game.h"

cGame::cGame()
	: World(nullptr)
	, Player(nullptr)
	, EntityMgr(nullptr)
	, GlobalEventHandler(nullptr)
	, ItemMgr(nullptr)
	, environmentMgr(nullptr)
	, gameHud(nullptr)
{
	World				= new cWorld;
	environmentMgr		= new cEnvironmentManager;
	EntityMgr			= new EntityManager;
	GlobalEventHandler = new GameEventHandler;
	gameHud				= new gameHUD;
	//ItemManager		= new cItemManager;
}

cGame::~cGame()
{
	delete ItemMgr;
	delete GlobalEventHandler;
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
