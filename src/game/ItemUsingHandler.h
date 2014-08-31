#pragma once

#include <SimplePhysicsEngine/PhysicsEngine.h>

#include "player/player.h"

class Item;

class BlockItem;
class MiscItem;
class WeaponItem;

class cPlayer;
class cSimplePhysicsEngine;
class cWorld;

class ItemUsingsHandler
{
public:
	ItemUsingsHandler(cPlayer * _p, cWorld * _w);
	~ItemUsingsHandler();

	bool UseBlockItem(BlockItem * block);

private:
	cPlayer * pPlayer;
	cWorld * pWorld;

};