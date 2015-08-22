#pragma once

//MY INCLUDES
#include <Utilities/cRefcounted.h>
#include <MathLib/math.h>

//STL INCLUDES
#include <vector>

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

        MathLib::Vector3F GetPosition() const;
        void SetPosition(const MathLib::Vector3F & val);

        MathLib::Vector3F GetScale() const;
        void SetScale(const MathLib::Vector3F & val);
        void SetScale(float val);

        MathLib::qaFloat GetRotation() const;
        void SetRotation(const MathLib::qaFloat & val);
        void RotateBy(const MathLib::qaFloat & val);

        SceneGraph * GetSceneGraph() const;
        void SetSceneGraph(SceneGraph * val);

        virtual MathLib::mat4f GetWorldMatrix() const;

        virtual MathLib::AABB GetBBox() const = 0;

    protected:
        virtual void PushToRQ(RenderQueue & rq) = 0;
        virtual int CheckVisibility(CameraSceneNode * activeCam) const;

        void CreateWorldMatrix();

        SceneGraph * pSceneGraph;

    private:
        MathLib::Vector3F Position;
        MathLib::Vector3F Scale;
        MathLib::qaFloat Rotation;

        MathLib::mat4f worldMatrix;

        ISceneNode * parent;
        ChildsList children;    //semi-automatic shared ptrs

    };
}
