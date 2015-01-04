#pragma once

#include "InteractableGameObject.h"

#include "../Items/ItemManager.h"

class CollectableGameObject : public InteractableGameObject
{
public:
	CollectableGameObject(ShiftEngine::MeshNode * sceneNode, item_id_t itemId, size_t count);
	~CollectableGameObject();

	virtual InteractionType GetInteraction() const override;

	item_id_t GetItemId() const;
	size_t GetCount() const;

	void Update(double dt) override;

private:
	item_id_t itemId;
	size_t count;

};
