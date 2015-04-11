#pragma once

#include "../APIs/D3D10/D3D10Mesh.h"
#include "../Material.h"

#include <MathLib/math.h>

namespace ShiftEngine
{
    class ISceneNode;
    class MeshNode;
    class CameraSceneNode;

    class SkySceneNode : public MeshNode
    {
    public:
        SkySceneNode(const Material * material, const IMeshDataPtr & meshData);
        ~SkySceneNode();

        virtual void PushToRQ(RenderQueue & rq);
        virtual int CheckVisibility(CameraSceneNode * activeCam) const;

    };
}
