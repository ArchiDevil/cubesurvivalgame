#pragma once

#include <MathLib/math.h>
#include <GraphicsEngine/SceneGraph/MeshNode.h>

#include "../events.h"
#include "EntityState.h"

#include <stack>
#include <memory>

class GameObject
{
public:
	GameObject(ShiftEngine::MeshNode * sceneNode);
	virtual ~GameObject();

	virtual Vector3F GetPosition() const;
	virtual void SetPosition(const Vector3F & Position);
	virtual void Update(double dt) = 0;
	virtual bool OnGameEvent(IGameEvent * ev);
	virtual bool CanBeHighlighted(const MathLib::Ray &uprojectedRay);
	virtual void Highlight();
	virtual void UnHightlight();

	ShiftEngine::MeshNode * GetSceneNode();

	int GetHealth() const;
	void SetHealth(int in_health);

	//service methods
	bool MustBeDeleted() const;
	void Delete();

	virtual void PushState(const std::shared_ptr<IEntityState> & state);
	virtual const EntityState GetCurrentState() const;

protected:
	ShiftEngine::MeshNode * SceneNode;
	bool ToDelete;

	int health;
	std::stack<std::shared_ptr<IEntityState>> states;

};

typedef std::shared_ptr<GameObject>	GameObjectPtr;
