#pragma once

#include "GameObject.h"

enum EntityState
{
	ES_Waiting,
	ES_Rotating,
	ES_Moving
};

class LivingGameObject : public GameObject
{
public:
	LivingGameObject(ShiftEngine::MeshNode * sceneNode);
	virtual ~LivingGameObject();

	virtual void Update(double dt) override;
	virtual bool Go(const MathLib::Vector2F & target);
	virtual MathLib::Vector2F GetTargetPosition() const;
	virtual EntityState GetCurrentState() const;

protected:
	EntityState currentState;
	MathLib::Vector2F targetPosition;

};