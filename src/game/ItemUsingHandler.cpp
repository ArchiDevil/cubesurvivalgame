#include "ItemUsingHandler.h"

#include "Items/BlockItem.h"
#include "Items/MiscItem.h"
#include "Items/WeaponItem.h"
#include "Items/EntityItem.h"
#include "Items/FoodItem.h"

#include "player/player.h"

ItemUsingsHandler::ItemUsingsHandler( cPlayer * pPlayer, cWorld * pWorld )
	: pPlayer(pPlayer), pWorld(pWorld)
{
}

bool ItemUsingsHandler::UseBlockItem( BlockItem * block )
{
	if(block->GetBlockID() == ID_AIR) return false;

	auto p = pPlayer->GetSelectedBlockPtr()->GetPositions();

	if(p.free.z > -1) //z не бывает меньше 0
	{
		pWorld->GetDataStorage()->GetBlock((int)p.free.x, (int)p.free.y, (int)p.free.z)->TypeID = block->GetBlockID();
		if(Physics::IsPlayerCollidesWithWorld(pPlayer->GetPosition(), pPlayer->GetHeight(), pWorld->GetDataStorage()) 
			&& !cSimplePhysicsEngine::GetInstance().IsPlayerFree())
		{
			pWorld->GetDataStorage()->GetBlock((int)p.free.x, (int)p.free.y, (int)p.free.z)->TypeID = ID_AIR;
			return false;
		}
		pWorld->GetDataStorage()->GetBlock((int)p.free.x, (int)p.free.y, (int)p.free.z)->TypeID = ID_AIR;
		pWorld->PlaceBlock((int)p.free.x, (int)p.free.y, (int)p.free.z, block->GetBlockID());
		return true;
	}

	return false;
}

bool ItemUsingsHandler::UseEntityItem(EntityItem * item)
{
	return false;
}

bool ItemUsingsHandler::UseFoodItem(FoodItem * item)
{
	pPlayer->SetHunger(pPlayer->GetHunger() + item->GetHunger());
	return true;
}

bool ItemUsingsHandler::UseWeaponItem(WeaponItem * item)
{
	return false;
}
