#pragma once

#include "Items/cItem.h"
#include "player/player.h"
#include "../SimplePhysicsEngine/PhysicsEngine.h"

class cItem;

class cBlockItem;
class cToolItem;
class cPotionItem;
class cArmorItem;
class cMiscItem;
class cWeaponItem;

class cPlayer;
class cSimplePhysicsEngine;
class cWorld;

class cItemUsingsHandler
{
public:
	cItemUsingsHandler(cPlayer * _p, cWorld * _w);
	~cItemUsingsHandler();

	bool UseBlockItem(cBlockItem * block);
	bool UsePotionItem(cPotionItem * potion);

private:
	cPlayer * pPlayer;
	cWorld * pWorld;

};