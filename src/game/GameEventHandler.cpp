#include "GameEventHandler.h"

#include <MathLib/math.h>
#include <SimplePhysicsEngine/PhysicsEngine.h>

#include "game.h"
#include "gameHud.h"
#include "world/world.h"
#include "world/worldStorage.h"
#include "Items/ItemManager.h"
#include "Entities/PlayerGameObject.h"
#include "Entities/GameObjectsManager.h"
#include "Entities/GameObjectInventory.h"

#include <cassert>

void GameEventHandler::DispatchEvent(std::unique_ptr<IGameEvent> ev)
{
    if (!ev)
        return;

    switch (ev->GetType())
    {
    case GE_PlayerMoves:
        Process((PlayerMoves*)ev.get());
        break;
    case GE_PlayerAttacks:
        Process((PlayerAttacks*)ev.get());
        break;
    case GE_PlayerPicksItem:
        Process((PlayerPicksItem*)ev.get());
        break;
    case GE_PlayerDropsItem:
        Process((PlayerDropsItem*)ev.get());
        break;
    case GE_PlayerUsesItem:
        Process((PlayerUsesItem*)ev.get());
        break;
    case GE_LivingDies:
        Process((LivingDies*)ev.get());
        break;
    case GE_Heat:
        Process((HeatEvent*)ev.get());
    default:
        // have you missed some event type?
        assert(false);
        break;
    }

    auto pGame = LostIsland::GetGamePtr();
    pGame->entityMgr->DispatchEvent(std::move(ev));
}

void GameEventHandler::Process(PlayerUsesItem* ev)
{
    auto pGame = LostIsland::GetGamePtr();
    auto pItem = pGame->itemMgr->GetItemById(ev->item_id);
    if (!pItem)
        return;
    if (ev->self)
        pItem->UseOnPlayer();
    else
        pItem->UseInWorld();
}

void GameEventHandler::Process(PlayerMoves* /*ev*/)
{
    static int PlayerCX = 0;
    static int PlayerCY = 0;
    static int prePlayerCX = PlayerCX;
    static int prePlayerCY = PlayerCY;

    auto pGame = LostIsland::GetGamePtr();

    prePlayerCX = PlayerCX;
    prePlayerCY = PlayerCY;

    PlayerCX = (int)floor(pGame->player->GetPosition().x / (int)pGame->world->GetDataStorage()->GetChunkWidth());
    PlayerCY = (int)floor(pGame->player->GetPosition().y / (int)pGame->world->GetDataStorage()->GetChunkWidth());

    if (PlayerCX != prePlayerCX)
    {
        if (PlayerCX - prePlayerCX < 0)
            pGame->world->ShiftChunkX(-1);
        if (PlayerCX - prePlayerCX > 0)
            pGame->world->ShiftChunkX(1);
    }

    if (PlayerCY != prePlayerCY)
    {
        if (PlayerCY - prePlayerCY < 0)
            pGame->world->ShiftChunkY(-1);
        if (PlayerCY - prePlayerCY > 0)
            pGame->world->ShiftChunkY(1);
    }
}

void GameEventHandler::Process(PlayerAttacks* /*ev*/)
{
}

void GameEventHandler::Process(PlayerPicksItem* ev)
{
    auto pGame = LostIsland::GetGamePtr();
    pGame->player->GetInventoryPtr()->AddItem(ev->itemId, ev->count);
    pGame->gameHud->OnUserInventoryChange();
}

void GameEventHandler::Process(PlayerDropsItem* ev)
{
    LostIsland::GetGamePtr()->player->GetInventoryPtr()->RemoveItem(ev->slot);
    auto pos = LostIsland::GetGamePtr()->player->GetPosition();
    pos.z += 10.0f;
    LostIsland::GetGamePtr()->entityMgr->CreateItemEntity(pos, Vector3D(0.0, 0.0, 3.0), ev->itemId, ev->count);
}

void GameEventHandler::Process(LivingDies* ev)
{
    if (!ev->object)
        return;

    auto pos = ev->object->GetPosition();
    pos.z += 3.0f;
    auto items = ev->object->GetInventory()->GetItems();
    for (const auto & item : items)
    {
        if (!item.count)
            continue;

        Vector3D velocity(sin(rand() % 16), cos(rand() % 16), 1.0);
        velocity = MathLib::normalize(velocity);
        velocity *= 6.0f;
        LostIsland::GetGamePtr()->entityMgr->CreateItemEntity(pos, velocity, item.itemId, item.count);
    }
}

void GameEventHandler::Process(HeatEvent* ev)
{
    LostIsland::GetGamePtr()->player->DispatchEvent(ev);
}
