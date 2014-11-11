#pragma once

#include "GameObject.h"

class LivingGameObject : public GameObject
{
public:
	LivingGameObject(ShiftEngine::MeshNode * sceneNode);
	virtual ~LivingGameObject();

	virtual void Update(double dt) override;
	virtual bool Go(const MathLib::Vector2F & target);

};
