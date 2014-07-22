#pragma once

#include "world/world.h"
#include "player/player.h"
#include "Items/cItemManager.h"
#include "EntityManager.h"

using MathLib::Vector3I;
using MathLib::Vector3D;

class cPlayer;
class cWorld;
class cEntityManager;
class cItemManager;

class cGameEventHandler
{
public:
	void Initialize(cPlayer * _p, cWorld * _c, cEntityManager * _em, cItemManager * _im);

	//world events
	void onBlockRemoved(BlockType bt, Vector3I Pos);	//вызывается при удалении блока
	void onBlockAdded(BlockType bt);					//вызывается при добавлении блока

	//player events
	void onPlayerMoves();								//вызывается при движении игрока
	void onPlayerAttack();								//при атаке игроком
	bool onPlayerPicksItem(cItem * item);				//игрок берет предмет
	void onPlayerDropsItem(cItem * item);				//игрок бросает айтим
	
private:
	//use some more

	cPlayer * pPlayer;
	cWorld * pWorld;
	cEntityManager * pEM;
	cItemManager * pIM;
};