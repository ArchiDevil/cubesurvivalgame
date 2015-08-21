#pragma once

#include <memory>

class cWorld;
class PlayerGameObject;
class GameObjectsManager;
class GameEventHandler;
class ItemManager;
class CraftingManager;
class EnvironmentManager;
class GameHUD;

struct Game
{
    Game();
    ~Game();

    Game(const Game & ref) = delete;
    Game & operator=(const Game & ref) = delete;
    Game(Game && ref) = delete;
    Game & operator=(Game && ref) = delete;

    std::unique_ptr<cWorld> world = nullptr;
    PlayerGameObject * player = nullptr;
    std::unique_ptr<GameObjectsManager> entityMgr = nullptr;
    std::unique_ptr<GameEventHandler> gameEventHandler = nullptr;
    std::unique_ptr<ItemManager> itemMgr = nullptr;
    std::unique_ptr<CraftingManager> craftingMgr = nullptr;
    std::unique_ptr<EnvironmentManager> environmentMgr = nullptr;
    std::unique_ptr<GameHUD> gameHud = nullptr;

};

namespace LostIsland
{
    void CreateGame();
    void TerminateGame();

    Game * GetGamePtr();
}
