#pragma once

#include "../cMaterial.h"
#include "../cMesh.h"
#include "../RenderQueue.h"

#include "ISceneNode.h"
#include "CameraSceneNode.h"

namespace ShiftEngine
{
	class ISceneNode;
	class CameraSceneNode;

	class MeshNode : public ISceneNode
	{
	public:
		MeshNode(const MeshDataPtr & _data, const Material * mat, const MathLib::AABB & _bbox);
		virtual ~MeshNode();

		virtual int Render();
		virtual void PushToRQ( RenderQueue & rq );

		virtual MeshDataPtr GetDataPtr() const;
		virtual void SetDataPtr(MeshDataPtr data);

		bool IsVisible() const;
		void SetVisibility(bool vis);

		ShiftEngine::Material * GetMaterialPtr();
		void SetMaterial(const ShiftEngine::Material * val);

	private:
		Material material;
		MeshDataPtr Data;
		bool isVisible;

	};
}
