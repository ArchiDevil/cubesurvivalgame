#include "CrafterGameObject.h"

#include "../game.h"

CrafterGameObject::CrafterGameObject(ShiftEngine::MeshNode * meshNode, uint32_t craftingTimeMs)
	: UsableGameObject(meshNode)
	, state(Empty)
	, storedItem(nullptr)
	, craftingTime(craftingTimeMs)
	, elapsedTime(0)
{
}

CrafterGameObject::~CrafterGameObject()
{
}

void CrafterGameObject::Activate()
{
	//getting item from player's hand if crafter in empty state
	//otherwise it should return item to player or do nothing 
	cGame * pGame = LostIsland::GetGamePtr();
	cInventory * pInventory = pGame->Player->GetInventoryPtr();

	switch (state)
	{
	case Empty:
		{
			auto rhItem = pInventory->GetRightHandItem();
			if (rhItem.Item && rhItem.count != 0)
			{
				storedItem = rhItem.Item;
				rhItem.count--;
				pInventory->SetRightItemHand(rhItem);
				state = Crafting;
				elapsedTime = 0;
				MainLog.Message("Crafting started");
			}
			break;
		}
	case Crafting:
		return;
	case Done:
		//stub
		MainLog.Message("Crafting is done");
		break;
	}
}

void CrafterGameObject::Update(double dt)
{
	elapsedTime += int(dt * 1000.0);

	if (state == Crafting && elapsedTime >= craftingTime)
	{
		elapsedTime = 0;
		state = Done;
	}
}
