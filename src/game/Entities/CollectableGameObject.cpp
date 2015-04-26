#include "CollectableGameObject.h"

#include "../game.h"
#include "../world/world.h"
#include "../world/worldStorage.h"

CollectableGameObject::CollectableGameObject(ShiftEngine::MeshNode * sceneNode, item_id_t itemId, size_t count)
    : itemId(itemId)
    , count(count)
    , InteractableGameObject(sceneNode)
{
}

CollectableGameObject::~CollectableGameObject()
{
}

InteractionType CollectableGameObject::GetInteraction() const
{
    return InteractionType::Collecting;
}

item_id_t CollectableGameObject::GetItemId() const
{
    return itemId;
}

size_t CollectableGameObject::GetCount() const
{
    return count;
}

void CollectableGameObject::Update(double /*dt*/)
{
    auto pGame = LostIsland::GetGamePtr();
    auto bbox = sceneNode->GetBBox();
    int heights[4] = { 0 };
    int minX = (int)std::floor(bbox.bMin.x);
    int maxX = (int)std::floor(bbox.bMax.x);
    int minY = (int)std::floor(bbox.bMin.y);
    int maxY = (int)std::floor(bbox.bMax.y);
    heights[0] = pGame->world->GetDataStorage()->GetFullHeight(minX, minY);
    heights[1] = pGame->world->GetDataStorage()->GetFullHeight(minX, maxY);
    heights[2] = pGame->world->GetDataStorage()->GetFullHeight(maxX, minY);
    heights[3] = pGame->world->GetDataStorage()->GetFullHeight(maxX, maxY);

    float maxHeight = (float)heights[0];
    for (int i = 0; i < 4; ++i)
        if (maxHeight < heights[i])
            maxHeight = (float)heights[i];

    auto position = GetPosition();
    position.z = (float)maxHeight;
    SetPosition(position);
}
