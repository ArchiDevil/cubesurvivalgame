#pragma once

#include "Entities/PlayerGameObject.h"
#include "world/world.h"
#include "Entities/EntityManager.h"
#include "Items/ItemManager.h"
#include "Environment/EnvironmentManager.h"
#include "cEventProc.h"
#include "gameHud.h"
#include "dayTimer.h"

struct cGame
{
	cGame();
	~cGame();

	cWorld					* World;
	PlayerGameObject		* Player;
	EntityManager			* EntityMgr;
	GameEventHandler		* GlobalEventHandler;
	ItemManager				* ItemMgr;
	cEnvironmentManager		* environmentMgr;
	gameHUD					* gameHud;

private:
	cGame(const cGame & ref);
	cGame operator=(const cGame & ref);
	cGame(cGame && ref);
	cGame operator=(cGame && ref);
};

namespace LostIsland
{
	void CreateGame();
	cGame * GetGamePtr();
}
