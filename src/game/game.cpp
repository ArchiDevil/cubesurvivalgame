#include "game.h"

cGame::cGame()
	: World(nullptr)
	, Player(nullptr)
	, EntityMgr(nullptr)
	, GameEventHandler(nullptr)
	, ItemMgr(nullptr)
	, environmentMgr(nullptr)
	, gameHud(nullptr)
{
	World				= new cWorld;
	environmentMgr		= new cEnvironmentManager;
	EntityMgr			= new EntityManager;
	GameEventHandler	= new cGameEventHandler;
	gameHud				= new gameHUD;
	//ItemManager		= new cItemManager;
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
