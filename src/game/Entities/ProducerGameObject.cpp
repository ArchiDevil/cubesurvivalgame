#include "ProducerGameObject.h"

#include "../game.h"

ProducerGameObject::ProducerGameObject(ShiftEngine::MeshNode * meshNode, Item * producedItem, uint32_t cycleTime)
	: UsableGameObject(meshNode)
	, produceType(producedItem)
	, cycleTime(cycleTime)
	, elapsedTime(0)
	, state(Producing)
{
}

ProducerGameObject::~ProducerGameObject()
{
}

void ProducerGameObject::Activate()
{
	auto * pGame = LostIsland::GetGamePtr();
	auto * pInventory = pGame->Player->GetInventoryPtr();

	if(state == Done)
	{
		// example of code for now
		SlotUnit item = pInventory->GetRightHandItem();
		item.count += 100;
		pInventory->SetRightItemHand(item);
		MainLog.Message("Added item into right hand");
		state = Producing;
		elapsedTime = 0;
	}
}

void ProducerGameObject::Update(double dt)
{
	elapsedTime += int(dt * 1000.0);

	if(state == Producing && elapsedTime >= cycleTime)
	{
		elapsedTime = 0;
		state = Done;
	}
}
