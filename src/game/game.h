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
    std::unique_ptr<CraftingManager> CratingMgr;
	std::unique_ptr<EnvironmentManager> environmentMgr;
	std::unique_ptr<gameHUD> gameHud;

};

namespace LostIsland
{
	void CreateGame();
	cGame * GetGamePtr();
}
