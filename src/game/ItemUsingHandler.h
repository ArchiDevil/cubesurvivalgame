#pragma once

#include <SimplePhysicsEngine/PhysicsEngine.h>

class Item;

class BlockItem;
class MiscItem;
class WeaponItem;
class EntityItem;
class FoodItem;

class cPlayer;
class cSimplePhysicsEngine;
class cWorld;

class ItemUsingsHandler
{
public:
	ItemUsingsHandler(cPlayer * pPlayer, cWorld * pWorld);

	bool UseBlockItem(BlockItem * item);
	bool UseEntityItem(EntityItem * item);
	bool UseFoodItem(FoodItem * item);
	bool UseWeaponItem(WeaponItem * item);

private:
	cPlayer * pPlayer;
	cWorld * pWorld;

};
