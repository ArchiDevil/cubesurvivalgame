#pragma once

#include "../cMesh.h"
#include "../cMaterial.h"

#include "../../MathLib/math.h"

namespace ShiftEngine
{
	class ISceneNode;
	class MeshNode;
	class CameraSceneNode;

	class SkySceneNode : public MeshNode
	{
	public:
		SkySceneNode(const Material * material, const MeshDataPtr & meshData);
		~SkySceneNode();

		virtual void PushToRQ( RenderQueue & rq );
		virtual int CheckVisibility( CameraSceneNode * activeCam ) const;

	};
}
