#pragma once

#include <MathLib/math.h>
#include <GraphicsEngine/SceneGraph/MeshNode.h>

class GameObject
{
public:
	GameObject(ShiftEngine::MeshNode * sceneNode);
	virtual ~GameObject();

	virtual Vector3F GetPosition() const;
	virtual void SetPosition(const Vector3F & Position);
	virtual void Update(double dt) = 0;

	ShiftEngine::MeshNode * GetSceneNode();

	//service methods
	bool MustBeDeleted() const;
	void Delete();

protected:
	ShiftEngine::MeshNode * SceneNode;
	bool ToDelete;

};