#pragma once

#include <cstdint>

#include <MathLib/math.h>

class PlayerGameObject;
class cWorld;
class EntityManager;
class ItemManager;

class GameEventHandler
{
public:
	//player events
	void onPlayerMoves(double dt);
	void onPlayerAttack();
	bool onPlayerPicksItem(uint64_t itemId);
	void onPlayerDropsItem(uint64_t itemId);
	void onPlayerUsesItem(bool self);
};
