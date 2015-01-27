#pragma once

//MY INCLUDES
#include <Utilities/cRefcounted.h>
#include <MathLib/math.h>

//STL INCLUDES
#include <vector>

//D3D INCLUDES
#include <d3dx10.h>

using MathLib::Vector2F;
using MathLib::Vector3F;
using MathLib::qaFloat;

namespace ShiftEngine
{
    class SceneGraph;
    class RenderQueue;
    class CameraSceneNode;

    class ISceneNode : public Refcounted
    {
    public:
        typedef std::vector<ISceneNode*> ChildsList;

        ISceneNode();
        virtual ~ISceneNode();

        void Draw(RenderQueue & rq);

        virtual void AddChild(ISceneNode * node);
        void RemoveChild(ISceneNode * node);
        const ChildsList & GetChilds() const;
        virtual void KillSelf();

        ISceneNode * GetParent() const;
        void SetParent(ISceneNode * _parent);
        void RemoveParent();

        Vector3F GetPosition() const;
        void SetPosition(const Vector3F & val);

        Vector3F GetScale() const;
        void SetScale(const Vector3F & val);
        void SetScale(float val);

        qaFloat GetRotation() const;
        void SetRotation(const qaFloat & val);
        void RotateBy(const qaFloat & val);

        SceneGraph * GetSceneGraph() const;
        void SetSceneGraph(SceneGraph * val);

        virtual D3DXMATRIX GetWorldMatrix() const;

        virtual MathLib::AABB GetBBox() const = 0;

    protected:
        virtual void PushToRQ(RenderQueue & rq) = 0;
        virtual int CheckVisibility(CameraSceneNode * activeCam) const;

        void CreateWorldMatrix();

        SceneGraph * pSceneGraph;

    private:
        Vector3F Position;
        Vector3F Scale;
        qaFloat Rotation;

        D3DXMATRIX worldMatrix;

        ISceneNode * parent;
        ChildsList children;    //semi-automatic shared ptrs

    };
}
