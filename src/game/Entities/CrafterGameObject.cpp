#include "CrafterGameObject.h"

#include "../game.h"
//#include "../GameObjectInventory.h"

CrafterGameObject::CrafterGameObject(ShiftEngine::MeshNode * meshNode, uint32_t craftingTimeMs)
	: GameObject(meshNode)
	, state(Empty)
	, craftingTime(craftingTimeMs)
	, elapsedTime(0)
	, storedItem(0)
{
}

CrafterGameObject::~CrafterGameObject()
{
}

void CrafterGameObject::Activate()
{
	////getting item from player's hand if crafter in empty state
	////otherwise it should return item to player or do nothing 
	//cGame * pGame = LostIsland::GetGamePtr();
	//GameObjectInventory * pInventory = pGame->Player->GetInventoryPtr();

	//switch (state)
	//{
	//case Empty:
	//	{
	//		SlotUnit * rhItem = pInventory->GetHandPtr();
	//		if (rhItem->itemId != 0)
	//		{
	//			storedItem = rhItem->itemId;
	//			if (--rhItem->count == 0)
	//				rhItem->itemId = 0;
	//			state = Crafting;
	//			elapsedTime = 0;
	//			LOG_INFO("Crafting started");
	//		}
	//		break;
	//	}
	//case Crafting:
	//	return;
	//case Done:
	//	//stub
	//	LOG_INFO("Crafting is done");
	//	break;
	//}
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
