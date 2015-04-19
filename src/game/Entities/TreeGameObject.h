#pragma once

#include "LiveGameObject.h"
#include "../game.h"

class TreeGameObject : public LiveGameObject
{
public:
    TreeGameObject(ShiftEngine::MeshNode * mesh);
    virtual ~TreeGameObject();

};
