#pragma once

#include "LiveGameObject.h"
#include "EntityActions.h"

#include <queue>
#include <memory>

class ControllableGameObject : public LiveGameObject
{
public:
    ControllableGameObject(ShiftEngine::MeshNode * sceneNode);
    virtual ~ControllableGameObject();

    virtual void Update(double dt) override;
    virtual bool Go(const MathLib::Vector2F & target);
    virtual void Stop();
    virtual void PushCommand(std::unique_ptr<IEntityAction> action);
    virtual void Interact(InteractableGameObject * target, InteractionType interaction);
    virtual void CancelCurrentCommand();

private:
    void CancelCommands();
    virtual void OnStateChange(EntityState from, EntityState to) override;

    std::queue<std::unique_ptr<IEntityAction>> Actions;

};
