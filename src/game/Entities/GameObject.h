#pragma once

#include <MathLib/math.h>
#include <GraphicsEngine/SceneGraph/MeshNode.h>

#include "EntityState.h"
#include "EntityActions.h"
#include "GameObjectInventory.h"

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
	virtual bool CanBeHighlighted(const MathLib::Ray &uprojectedRay);
	virtual void Highlight();
	virtual void UnHightlight();
	virtual std::unique_ptr<IEntityAction> GetInteraction();

	ShiftEngine::MeshNode * GetSceneNode();

	int GetHealth() const;
	void SetHealth(int in_health);

	//service methods
	bool MustBeDeleted() const;
	void Delete();

	virtual bool DispatchState(std::unique_ptr<IEntityState> state);
	virtual const EntityState GetCurrentState() const;

protected:
	virtual void OnStateChange(EntityState from, EntityState to);

	ShiftEngine::MeshNode * SceneNode;
	std::unique_ptr<IEntityState> currentState;
	bool ToDelete;
	int health;

	GameObjectInventory inventory;

};

typedef std::shared_ptr<GameObject>	GameObjectPtr;
