#include "game.h"

cGame::cGame()
{
	World				= new cWorld;
	Player				= new cPlayer;
	environmentMgr		= new cEnvironmentManager;
	EntityManager		= new cEntityManager;
	GameEventHandler	= new cGameEventHandler;
	gameHud				= new gameHUD;
	//ItemManager			= new cItemManager;
	//invScreen			= new InventoryScreen;
}

cGame::~cGame()
{
	delete invScreen;
	delete ItemMgr;
	delete GameEventHandler;
	delete World;
	delete Player;
	delete EntityManager;
	delete gameHud;
}