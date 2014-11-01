#pragma once

#include "GameObject.h"

enum EntityState
{
	ES_Waiting,
	ES_Rotating,
	ES_Moving,
	ES_Attacking,
	ES_Dying
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

	uint32_t GetHealth() const;
	void SetHealth(uint32_t health);

protected:
	EntityState currentState;
	MathLib::Vector2F targetPosition;
	uint32_t healthPoints;

};
