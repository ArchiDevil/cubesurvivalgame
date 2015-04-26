#include "game.h"

#include "world/world.h"
#include "Entities/PlayerGameObject.h"
#include "Entities/GameObjectsManager.h"
#include "Items/ItemManager.h"
#include "Environment/EnvironmentManager.h"
#include "GameEventHandler.h"
#include "gameHud.h"
#include "CraftingManager.h"

Game::Game()
    : player(nullptr)
{
    world.reset(new cWorld);
    environmentMgr.reset(new EnvironmentManager);
    entityMgr.reset(new GameObjectsManager);
    gameEventHandler.reset(new GameEventHandler);
    itemMgr.reset(new ItemManager());
    craftingMgr.reset(new CraftingManager);
}

Game::~Game()
{
}

static Game * GamePtr = nullptr;

void LostIsland::CreateGame()
{
    delete GamePtr;
    GamePtr = new Game();
}

Game * LostIsland::GetGamePtr()
{
    return GamePtr;
}
