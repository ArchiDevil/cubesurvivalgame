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
	void onBlockRemoved(BlockType bt, MathLib::Vector3I Pos);	//вызывается при удалении блока
	void onBlockAdded(BlockType bt);							//вызывается при добавлении блока

	//player events
	void onPlayerMoves(double dt);								//вызывается при движении игрока
	void onPlayerAttack();										//при атаке игроком
	bool onPlayerPicksItem(uint64_t itemId);					//игрок берет предмет
	void onPlayerDropsItem(uint64_t itemId);					//игрок бросает айтим
};
