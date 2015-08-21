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
{
    world.reset(new cWorld());
    environmentMgr.reset(new EnvironmentManager());
    entityMgr.reset(new GameObjectsManager());
    gameEventHandler.reset(new GameEventHandler());
    itemMgr.reset(new ItemManager());
    craftingMgr.reset(new CraftingManager());
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

void LostIsland::TerminateGame()
{
    delete GamePtr;
    GamePtr = nullptr;
}

Game * LostIsland::GetGamePtr()
{
    return GamePtr;
}
