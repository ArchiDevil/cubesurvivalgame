#pragma once

#include "../Material.h"
#include "../APIs/D3D10/D3D10Mesh.h"
#include "../RenderQueue.h"

#include "ISceneNode.h"

namespace ShiftEngine
{
    class ISceneNode;
    class CameraSceneNode;

    class MeshNode : public ISceneNode
    {
    public:
        MeshNode(const IMeshDataPtr & _data, const Material * mat, const MathLib::AABB & _bbox);
        virtual ~MeshNode();

        virtual int Render();
        virtual void PushToRQ(RenderQueue & rq);

        virtual IMeshDataPtr GetDataPtr() const;
        virtual void SetDataPtr(IMeshDataPtr data);

        bool IsVisible() const;
        void SetVisibility(bool vis);

        ShiftEngine::Material * GetMaterialPtr();
        void SetMaterial(const ShiftEngine::Material * val);

        virtual MathLib::AABB GetBBox() const override;
        void SetBBox(const MathLib::AABB &bbox);

    private:
        MathLib::AABB bbox;
        Material material;
        IMeshDataPtr Data;
        bool isVisible;

    };
}
