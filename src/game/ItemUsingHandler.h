#pragma once

#include "Items/Item.h"
#include "player/player.h"
#include "../SimplePhysicsEngine/PhysicsEngine.h"

class Item;

class BlockItem;
class ToolItem;
class PotionItem;
class ArmorItem;
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
	bool UsePotionItem(PotionItem * potion);

private:
	cPlayer * pPlayer;
	cWorld * pWorld;

};