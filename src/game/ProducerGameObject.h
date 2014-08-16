#pragma once

#include "UsableGameObject.h"
#include "Items/Item.h"

class ProducerGameObject : public UsableGameObject
{
public:
	ProducerGameObject(ShiftEngine::MeshNode * meshNode);
	virtual ~ProducerGameObject();

	// UsableGameObject
	virtual void Activate() override;

	// GameObject
	virtual void Update(double dt) override;

private:
	Item * produceType;

};
