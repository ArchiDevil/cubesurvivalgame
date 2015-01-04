#pragma once

#include "Entities/PlayerGameObject.h"
#include "world/world.h"
#include "Entities/GameObjectsManager.h"
#include "Items/ItemManager.h"
#include "Environment/EnvironmentManager.h"
#include "GameEventHandler.h"
#include "gameHud.h"

#include <memory>

struct cGame
{
	cGame();
	~cGame();

	cGame(const cGame & ref) = delete;
	cGame & operator=(const cGame & ref) = delete;
	cGame(cGame && ref) = delete;
	cGame & operator=(cGame && ref) = delete;

	std::unique_ptr<cWorld> World;
	PlayerGameObject * Player;
	std::unique_ptr<GameObjectsManager> EntityMgr;
	std::unique_ptr<GameEventHandler> GlobalEventHandler;
	std::unique_ptr<ItemManager> ItemMgr;
	std::unique_ptr<EnvironmentManager> environmentMgr;
	std::unique_ptr<gameHUD> gameHud;

};

namespace LostIsland
{
	void CreateGame();
	cGame * GetGamePtr();
}
