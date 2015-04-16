#include "HeaterGameObject.h"

#include "../game.h"
#include "../GameEventHandler.h"

HeaterGameObject::HeaterGameObject(ShiftEngine::MeshNode * sceneNode, int heatValue)
    : GameObject(sceneNode)
    , heatValue(heatValue)
{
}

HeaterGameObject::~HeaterGameObject()
{
}

void HeaterGameObject::Update(double dt)
{
    static double accumulatedTime = 0.0;
    accumulatedTime += dt;
    if (accumulatedTime > 1.0)
    {
        LostIsland::GetGamePtr()->GlobalEventHandler->DispatchEvent(std::make_unique<HeatEvent>(GetPosition(), heatValue));
        accumulatedTime = 0.0;
    }
}

int HeaterGameObject::GetHeatValue() const
{
    return heatValue;
}
