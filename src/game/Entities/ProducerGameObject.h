#pragma once

#include "GameObject.h"
#include "../Items/Item.h"

// for now, this class creates item inside it every N milliseconds
// player can get item from here and use them

class ProducerGameObject : public GameObject
{
	enum ProducerState
	{
		Producing,
		Done
	};

public:
	ProducerGameObject(ShiftEngine::MeshNode * meshNode, Item * producedItem, uint32_t cycleTimeMs);
	virtual ~ProducerGameObject();

	virtual void Activate();

	// GameObject
	virtual void Update(double dt) override;

private:
	ProducerState state;
	const uint32_t cycleTime;
	uint32_t elapsedTime;
	Item * produceType;

};

typedef std::shared_ptr<ProducerGameObject> ProducerPtr;
