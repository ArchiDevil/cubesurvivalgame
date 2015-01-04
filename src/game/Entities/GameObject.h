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

	// pure virtuals 
	virtual void Update(double dt) = 0;

	// setters
	virtual void SetPosition(const Vector3F & Position);

	// getters
	virtual Vector3F GetPosition() const;
	ShiftEngine::MeshNode * GetSceneNode();

	// highlight methods
	virtual bool CanBeHighlighted(const MathLib::Ray &uprojectedRay);
	virtual void Highlight();
	virtual void UnHightlight();

	// service methods
	bool MustBeDeleted() const;
	void Delete();

protected:
	ShiftEngine::MeshNode * SceneNode;
	bool ToDelete;

};

typedef std::shared_ptr<GameObject>	GameObjectPtr;
