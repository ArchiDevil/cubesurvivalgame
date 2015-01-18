#pragma once

#include <cstdint>

#include "Items/ItemManager.h"

class GameObjectInventory;
class LiveGameObject;

enum GameEventType
{
	GE_PlayerMoves,
	GE_PlayerAttacks,
	GE_PlayerPicksItem,
	GE_PlayerDropsItem,
	GE_PlayerUsesItem,
	GE_LivingDies,
};

struct GameEvent
{
	virtual GameEventType GetType() const = 0;
};

class GameEventHandler
{
public:
	void DispatchEvent(GameEvent * ev);

	//player events
	void onPlayerMoves(double dt);
	bool onPlayerPicksItem(uint64_t itemId, size_t count);
	void onPlayerDropsItem(unsigned slot, uint64_t itemId, size_t count);
	void onLivingObjectDies(LiveGameObject * object);
	void onPlayerUsesItem(bool self, item_id_t itemId);
};
