#pragma once

#include <memory>

#include "GameEvents.h"

class GameObjectInventory;
class LiveGameObject;

class GameEventHandler
{
public:
	void DispatchEvent(std::unique_ptr<IGameEvent> ev);

private:
    void Process(IGameEvent* ev);
    void Process(PlayerUsesItem* ev);
    void Process(PlayerMoves* ev);
    void Process(PlayerAttacks* ev);
    void Process(PlayerPicksItem* ev);
    void Process(PlayerDropsItem* ev);
    void Process(LivingDies* ev);
};
