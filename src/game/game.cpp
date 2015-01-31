#include "game.h"

#include "world/world.h"
#include "Entities/PlayerGameObject.h"
#include "Entities/GameObjectsManager.h"
#include "Items/ItemManager.h"
#include "Environment/EnvironmentManager.h"
#include "GameEventHandler.h"
#include "gameHud.h"
#include "CraftingManager.h"

cGame::cGame()
	: Player(nullptr)
{
	World.reset(new cWorld);
	environmentMgr.reset(new EnvironmentManager);
	EntityMgr.reset(new GameObjectsManager);
	GlobalEventHandler.reset(new GameEventHandler);
	gameHud.reset(new gameHUD);
    ItemMgr.reset(new ItemManager());
    CratingMgr.reset(new CraftingManager);
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
