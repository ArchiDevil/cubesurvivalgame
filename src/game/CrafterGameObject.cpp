#include "CrafterGameObject.h"

#include "game.h"

CrafterGameObject::CrafterGameObject(ShiftEngine::MeshNode * meshNode)
	: UsableGameObject(meshNode)
	, state(Empty)
	, storedItem(nullptr)
{
}

CrafterGameObject::~CrafterGameObject()
{
}

void CrafterGameObject::Activate()
{
	//getting item from player's hand if crafter in empty state
	//otherwise it should return item to player or do nothing 
	//cGame * pGame = Game::GetGamePtr();
	//cInventory * pInventory = pGame->Player->GetInventoryPtr();

	//switch (state)
	//{
	//case Empty:
	//{
	//	auto rhItem = pInventory->GetRightHandItem();
	//	if (rhItem.Item && rhItem.count != 0)
	//	{
	//		storedItem = rhItem.Item;
	//		rhItem.count--;
	//		pInventory->SetRightItemHand(rhItem);
	//		state = Crafting;
	//		MainLog.Message("Crafting started");
	//	}
	//	break;
	//}
	//case Crafting:
	//	return;
	//case Done:
	//	//stub
	//	MainLog.Message("Crafting is done");
	//	break;
	//}
}

void CrafterGameObject::Update(double dt)
{
	if (state == Crafting)
		state = Done;
}
