#pragma once

#include "ControllableGameObject.h"
#include "GameObjectInventory.h"

#include <MathLib/math.h>

class PlayerGameObject final : public ControllableGameObject
{
public:
    PlayerGameObject(ShiftEngine::MeshNode * sceneNode, ItemManager * pItemMgr);
    ~PlayerGameObject() = default;

    unsigned int GetHunger() const;
    unsigned int GetWarmth() const;

    void SetHunger(unsigned int hunger);
    void SetWarmth(unsigned int temperature);

    PlayerInventory * GetInventoryPtr();

    void Update(double dt) override;
    bool Go(const MathLib::Vector2F & target) override;
    void Attack(LiveGameObject * target) const override;
    InteractionType GetInteraction() const override;
    void DispatchEvent(const IGameEvent *ev) override;

private:
    PlayerInventory Inventory;

    unsigned int hunger;
    unsigned int warmth;

    ShiftEngine::MeshNode * targetMarker;

};

typedef std::shared_ptr<PlayerGameObject> PlayerPtr;
