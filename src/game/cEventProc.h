#pragma once

#include "world/world.h"
#include "player/player.h"
#include "Items/ItemManager.h"
#include "Entities/EntityManager.h"

using MathLib::Vector3I;
using MathLib::Vector3D;

class cPlayer;
class cWorld;
class EntityManager;
class ItemManager;

class cGameEventHandler
{
public:
	void Initialize(cPlayer * _p, cWorld * _c, EntityManager * _em, ItemManager * _im);

	//world events
	void onBlockRemoved(BlockType bt, Vector3I Pos);	//вызывается при удалении блока
	void onBlockAdded(BlockType bt);					//вызывается при добавлении блока

	//player events
	void onPlayerMoves();								//вызывается при движении игрока
	void onPlayerAttack();								//при атаке игроком
	bool onPlayerPicksItem(Item * item);				//игрок берет предмет
	void onPlayerDropsItem(Item * item);				//игрок бросает айтим
	
private:
	//use some more

	cPlayer * pPlayer;
	cWorld * pWorld;
	EntityManager * pEM;
	ItemManager * pIM;
};