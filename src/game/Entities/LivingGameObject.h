#pragma once

#include "GameObject.h"
#include "EntityActions.h"

#include <queue>
#include <memory>

class LivingGameObject : public GameObject
{
public:
	LivingGameObject(ShiftEngine::MeshNode * sceneNode);
	virtual ~LivingGameObject();

	virtual void Update(double dt) override;
	virtual bool Go(const MathLib::Vector2F & target);
	virtual void Stop();
	virtual void PushCommand(std::unique_ptr<IEntityAction> action);
	virtual void CancelCurrentCommand();

private:
	void CancelCommands();
	virtual void OnStateChange(EntityState from, EntityState to) override;

	std::queue<std::unique_ptr<IEntityAction>> Actions;

};
