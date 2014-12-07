#pragma once

#include "Entities/PlayerGameObject.h"
#include "world/world.h"
#include "Entities/EntityManager.h"
#include "Items/ItemManager.h"
#include "Environment/EnvironmentManager.h"
#include "GameEventHandler.h"
#include "gameHud.h"
#include "dayTimer.h"

struct cGame
{
	cGame();
	~cGame();

	cGame(const cGame & ref) = delete;
	cGame & operator=(const cGame & ref) = delete;
	cGame(cGame && ref) = delete;
	cGame & operator=(cGame && ref) = delete;

	cWorld					* World;
	PlayerGameObject		* Player;
	EntityManager			* EntityMgr;
	GameEventHandler		* GlobalEventHandler;
	ItemManager				* ItemMgr;
	cEnvironmentManager		* environmentMgr;
	gameHUD					* gameHud;

};

namespace LostIsland
{
	void CreateGame();
	cGame * GetGamePtr();
}
