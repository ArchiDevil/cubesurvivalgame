#include "cItemUsingHandler.h"

#include "Items/cArmorItem.h"
#include "Items/cBlockItem.h"
#include "Items/cMiscItem.h"
#include "Items/cPotionItem.h"
#include "Items/cToolItem.h"
#include "Items/cWeaponItem.h"

cItemUsingsHandler::cItemUsingsHandler( cPlayer * _p, cWorld * _w )
	: pPlayer(_p), pWorld(_w)
{
}

cItemUsingsHandler::~cItemUsingsHandler()
{
}

bool cItemUsingsHandler::UseBlockItem( cBlockItem * block )
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

bool cItemUsingsHandler::UsePotionItem( cPotionItem * potion )
{
	int newHP = pPlayer->GetHealth() + potion->GetHealthPoints();
	int newFP = pPlayer->GetLongFatique() + potion->GetFatiquePoints();
	int HungP = pPlayer->GetHunger() + potion->GetHungerPoints();

	pPlayer->SetHealth(newHP);
	pPlayer->SetLongFatique(newFP);
	pPlayer->SetHunger(HungP);

	//how to use quality?

	return true;
}
