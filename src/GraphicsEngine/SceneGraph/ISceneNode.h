#pragma once

//STL INCLUDES
#include <string>
#include <vector>

//D3D INCLUDES
#include <d3dx10.h>

//MY INCLUDES
#include "../../Utilities/cRefcounted.h"
#include "../../MathLib/math.h"

using MathLib::Vector2F;
using MathLib::Vector3F;

namespace ShiftEngine
{
	class SceneGraph;
	class RenderQueue;
	class CameraSceneNode;

	class ISceneNode : public Refcounted
	{
	public:
		typedef std::vector<ISceneNode*> ChildsList;

		ISceneNode(const MathLib::AABB & _bbox);
		virtual ~ISceneNode();

		void Draw(RenderQueue & rq);

		virtual void AddChild(ISceneNode * node);
		void RemoveChild(ISceneNode * node);

		ISceneNode * GetParent() const;
		void SetParent(ISceneNode * _parent);
		void RemoveParent();

		Vector3F GetPosition() const;
		void SetPosition(const Vector3F & val);

		Vector3F GetScale() const;
		void SetScale(const Vector3F & val);

		Vector3F GetRotation() const;
		void SetRotation(const Vector3F & val);

		SceneGraph * GetSceneGraph() const;
		void SetSceneGraph(SceneGraph * val);

		virtual D3DXMATRIX GetWorldMatrix() const;
		MathLib::AABB GetBBox() const;

		virtual void KillSelf();

		const ChildsList & GetChilds() const;

	protected:
		virtual void PushToRQ(RenderQueue & rq) = 0;
		virtual int CheckVisibility(CameraSceneNode * activeCam) const;

		void CreateWorldMatrix();

		MathLib::AABB bbox;
		SceneGraph * pSceneGraph;

	private:
		Vector3F Position;
		Vector3F Scale;
		Vector3F Rotation;	//euler angles

		D3DXMATRIX worldMatrix;

		ISceneNode * parent;
		ChildsList children;	//semi-automatic shared ptrs

	};
}
