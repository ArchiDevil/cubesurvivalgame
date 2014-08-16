#include "ProducerGameObject.h"

#include "game.h"

ProducerGameObject::ProducerGameObject(ShiftEngine::MeshNode * meshNode)
	: UsableGameObject(meshNode), produceType(nullptr)
{
}

ProducerGameObject::~ProducerGameObject()
{
}

void ProducerGameObject::Activate()
{
	//auto * pGame = Game::GetGamePtr();
	//auto * pInventory = pGame->Player->GetInventoryPtr();

	//// example of code for now
	//SlotUnit item = pInventory->GetRightHandItem();
	//item.count += 100;
	//pInventory->SetRightItemHand(item);
	//MainLog.Message("Added item into right hand");
}

void ProducerGameObject::Update(double /*dt*/)
{
}