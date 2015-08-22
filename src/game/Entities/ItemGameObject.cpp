#include "ItemGameObject.h"

#include "../game.h"
#include "../Entities/PlayerGameObject.h"
#include "../GameEventHandler.h"

ItemGameObject::ItemGameObject(uint64_t itemId, size_t count, PhysObjectPtr _obj, ShiftEngine::MeshNode * meshNode)
    : itemId(itemId)
    , count(count)
    , PhysicsGameObject(_obj, meshNode)
{
}

ItemGameObject::~ItemGameObject()
{
}

void ItemGameObject::Update(double dt)
{
    PhysicsGameObject::Update(dt);

    //  Vector3F rot = SceneNode->GetRotation();
    //  rot.z += dt;
    //  SceneNode->SetRotation(rot);

    Game * pGame = LostIsland::GetGamePtr();
    auto ppos = pGame->player->GetPosition();
    if (MathLib::distance(ppos, GetPosition()) < 1.0f)
    {
        LostIsland::GetGamePtr()->gameEventHandler->DispatchEvent(std::make_unique<PlayerPicksItem>(itemId, count));
        Delete();
    }
}

uint64_t ItemGameObject::GetItemId() const
{
    return itemId;
}
