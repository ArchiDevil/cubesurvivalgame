#pragma once

#include <MathLib/math.h>
#include <GraphicsEngine/SceneGraph/MeshNode.h>

#include "../events.h"

enum EntityState
{
	ES_Waiting,
	ES_Rotating,
	ES_Moving
};

class GameObject
{
public:
	GameObject(ShiftEngine::MeshNode * sceneNode);
	virtual ~GameObject();

	virtual Vector3F GetPosition() const;
	virtual void SetPosition(const Vector3F & Position);
	virtual void Update(double dt);
	virtual bool OnGameEvent(IGameEvent * ev);
	virtual void Unselect();
	virtual bool Select(const MathLib::Ray & unprojectedVector);
	ShiftEngine::MeshNode * GetSceneNode();
	virtual bool Go(const MathLib::Vector2F & target);

	//service methods
	bool MustBeDeleted() const;
	void Delete();

protected:
	ShiftEngine::MeshNode * SceneNode;
	bool ToDelete;

	EntityState currentState;
	float targetRotationAngle;
	MathLib::Vector2F targetPosition;

};
