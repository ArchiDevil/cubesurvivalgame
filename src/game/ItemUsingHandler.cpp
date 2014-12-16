#include "ItemUsingHandler.h"

#include "Items/MiscItem.h"
#include "Items/WeaponItem.h"
#include "Items/EntityItem.h"
#include "Items/FoodItem.h"

#include "Entities/PlayerGameObject.h"

ItemUsingsHandler::ItemUsingsHandler( PlayerGameObject * pPlayer, cWorld * pWorld )
	: pPlayer(pPlayer), pWorld(pWorld)
{
}

bool ItemUsingsHandler::UseEntityItem(EntityItem * /*item*/)
{
	return false;
}

bool ItemUsingsHandler::UseFoodItem(FoodItem * item)
{
	pPlayer->SetHunger(pPlayer->GetHunger() + item->GetHunger());
	return true;
}

bool ItemUsingsHandler::UseWeaponItem(WeaponItem * /*item*/)
{
	return false;
}
