#pragma once

#include "player\player.h"
#include "world\world.h"
#include "EntityManager.h"
#include "Items\cItemManager.h"
#include "Environment\EnvironmentManager.h"
#include "cEventProc.h"
#include "gameHud.h"
#include "dayTimer.h"
#include "InventoryScreen.h"

struct cGame
{
	cGame();
	~cGame();

	cWorld					* World;			//world class
	cPlayer					* Player;			//current player class
	cEntityManager			* EntityManager;	//entity manager
	cGameEventHandler		* GameEventHandler;	//events handler
	cItemManager			* ItemManager;		//items handler
	cEnvironmentManager		* environmentMgr;	//environment simulator
	gameHUD					* gameHud;			//hud

	InventoryScreen			* invScreen;		//inventory screen
};