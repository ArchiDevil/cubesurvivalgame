#pragma once

#include "GameObject.h"

class HeaterGameObject : public GameObject
{
public:
    HeaterGameObject(ShiftEngine::MeshNode * sceneNode, int heatValue);
    virtual ~HeaterGameObject();

    virtual void Update(double dt);
    int GetHeatValue() const;

private:
    int heatValue;

};
