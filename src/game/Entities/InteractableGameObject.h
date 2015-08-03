#pragma once

#include "GameObject.h"

enum class InteractionType
{
    Nothing,
    Collecting,
    Attacking,
    Fishing
    // etc, like crafting, activating and anything you want
};

class InteractableGameObject : public GameObject
{
public:
    InteractableGameObject(ShiftEngine::MeshNode * sceneNode)
        : GameObject(sceneNode)
    {
    }

    virtual ~InteractableGameObject()
    {
    }

    virtual InteractionType GetInteraction() const = 0;

};
