#pragma once

#include <memory>

class cWorld;
class PlayerGameObject;
class GameObjectsManager;
class GameEventHandler;
class ItemManager;
class CraftingManager;
class EnvironmentManager;
class gameHUD;

struct Game
{
    Game();
    ~Game();

    Game(const Game & ref) = delete;
    Game & operator=(const Game & ref) = delete;
    Game(Game && ref) = delete;
    Game & operator=(Game && ref) = delete;

    std::unique_ptr<cWorld> World;
    PlayerGameObject * Player;
    std::unique_ptr<GameObjectsManager> EntityMgr;
    std::unique_ptr<GameEventHandler> GlobalEventHandler;
    std::unique_ptr<ItemManager> ItemMgr;
    std::unique_ptr<CraftingManager> CratingMgr;
    std::unique_ptr<EnvironmentManager> environmentMgr;
    std::unique_ptr<gameHUD> gameHud;

};

namespace LostIsland
{
    void CreateGame();
    Game * GetGamePtr();
}
