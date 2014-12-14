#pragma once

#include "StaticGameObject.h"
#include "../game.h"

class TreeGameObject : public StaticGameObject
{
public:
	TreeGameObject(ShiftEngine::MeshNode * mesh);
	virtual ~TreeGameObject();

	virtual std::unique_ptr<IEntityAction> GetInteraction();

};
