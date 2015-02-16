#pragma once

#include <MathLib/math.h>

#include "Common.h"

enum GameEventType
{
    GE_PlayerMoves,
    GE_PlayerAttacks,
    GE_PlayerPicksItem,
    GE_PlayerDropsItem,
    GE_PlayerUsesItem,
    GE_LivingDies,
    GE_Heat,
};

class LiveGameObject;

struct IGameEvent
{
    virtual GameEventType GetType() const = 0;
};

struct PlayerUsesItem : public IGameEvent
{
    PlayerUsesItem(item_id_t item_id, bool self)
        : item_id(item_id)
        , self(self)
    {
    }

    GameEventType GetType() const override { return GE_PlayerUsesItem; }

    item_id_t item_id;
    bool self;
};

struct PlayerMoves : public IGameEvent
{
    PlayerMoves() {}
    GameEventType GetType() const override { return GE_PlayerMoves; }
};

struct PlayerAttacks : public IGameEvent
{
    PlayerAttacks() {}
    GameEventType GetType() const override { return GE_PlayerAttacks; }
};

struct PlayerPicksItem : public IGameEvent
{
    PlayerPicksItem(item_id_t itemId, size_t count)
        : itemId(itemId)
        , count(count)
    {
    }

    GameEventType GetType() const override { return GE_PlayerPicksItem; }

    item_id_t itemId;
    size_t count;
};

struct PlayerDropsItem : public IGameEvent
{
    PlayerDropsItem(unsigned slot, item_id_t itemId, size_t count)
        : slot(slot)
        , itemId(itemId)
        , count(count)
    {
    }

    GameEventType GetType() const override { return GE_PlayerDropsItem; }

    unsigned slot;
    item_id_t itemId;
    size_t count;
};

struct LivingDies : public IGameEvent
{
    LivingDies(LiveGameObject * object)
        : object(object)
    {
    }

    GameEventType GetType() const override { return GE_LivingDies; }

    LiveGameObject * object;
};

struct HeatEvent : public IGameEvent
{
    HeatEvent(const MathLib::Vector3F& heatSource, int heatValue)
        : heatSource(heatSource)
        , heatValue(heatValue)
    {
    }

    GameEventType GetType() const override { return GE_Heat; }

    MathLib::Vector3F heatSource;
    int heatValue;
};
