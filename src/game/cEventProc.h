#pragma once

#include <cstdint>

#include <MathLib/math.h>

class cPlayer;
class cWorld;
class EntityManager;
class ItemManager;

enum BlockType : unsigned char;

class cGameEventHandler
{
public:
	//world events
	void onBlockRemoved(BlockType bt, MathLib::Vector3I Pos);	
	void onBlockAdded(BlockType bt);							

	//player events
	void onPlayerMoves(double dt);
	void onPlayerAttack();
	bool onPlayerPicksItem(uint64_t itemId);
	void onPlayerDropsItem(uint64_t itemId);
	void onPlayerUsesItem(bool self);
};
