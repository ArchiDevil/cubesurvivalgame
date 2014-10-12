#pragma once

#include <SimplePhysicsEngine/PhysicsEngine.h>

class Item;

class BlockItem;
class MiscItem;
class WeaponItem;
class EntityItem;
class FoodItem;

class PlayerGameObject;
class cSimplePhysicsEngine;
class cWorld;

class ItemUsingsHandler
{
public:
	ItemUsingsHandler(PlayerGameObject * pPlayer, cWorld * pWorld);

	bool UseEntityItem(EntityItem * item);
	bool UseFoodItem(FoodItem * item);
	bool UseWeaponItem(WeaponItem * item);

private:
	PlayerGameObject * pPlayer;
	cWorld * pWorld;

};
