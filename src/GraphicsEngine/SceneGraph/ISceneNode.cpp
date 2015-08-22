#include "ISceneNode.h"

#include "CameraSceneNode.h"
#include "SceneGraph.h"

#include <algorithm>

ShiftEngine::ISceneNode::ISceneNode()
    : Scale(1.0f, 1.0f, 1.0f)
    , Rotation(MathLib::quaternionFromVecAngle(MathLib::Vector3F(0.0f, 0.0f, 1.0f), 0.0f))
    , Position(0.0f, 0.0f, 0.0f)
    , parent(nullptr)
    , pSceneGraph(nullptr)
{
    CreateWorldMatrix();
}

ShiftEngine::ISceneNode::~ISceneNode()
{
    for (auto child : children)
        child->release();
}

void ShiftEngine::ISceneNode::AddChild(ISceneNode * node)
{
    //TODO: fix memleak here
    if (!node)
        return;

    //every node must have only 1 parent

    node->addRef(); //to avoid killing by parent

    if (node->parent)
        node->parent->RemoveChild(node);

    node->parent = this;
    //find empty place
    for (auto & elem : children)
    {
        if (elem == nullptr)
        {
            elem = node;
            return;
        }
    }

    children.push_back(node);
}

void ShiftEngine::ISceneNode::RemoveChild(ISceneNode * node)
{
    node->release();
    auto iter = std::find(children.begin(), children.end(), node);
    if (iter != children.end())
    {
        *iter = nullptr;
        *iter = *(--children.end());
        children.pop_back();
    }
}

void ShiftEngine::ISceneNode::SetParent(ISceneNode * _parent)
{
    if (_parent)
        _parent->AddChild(this);    //it will set parent to node
    else
        parent = _parent;
}

void ShiftEngine::ISceneNode::RemoveParent()
{
    if (this->parent)
        parent->RemoveChild(this);
    parent = nullptr;
}

void ShiftEngine::ISceneNode::KillSelf()
{
    if (this->parent)
        parent->RemoveChild(this);
}

ShiftEngine::ISceneNode * ShiftEngine::ISceneNode::GetParent() const
{
    return parent;
}

const ShiftEngine::ISceneNode::ChildsList & ShiftEngine::ISceneNode::GetChilds() const
{
    return children;
}

void ShiftEngine::ISceneNode::Draw(RenderQueue & rq)
{
    this->PushToRQ(rq);

    if (!children.empty())
        for (auto child : children)
            child->Draw(rq);
}

MathLib::mat4f ShiftEngine::ISceneNode::GetWorldMatrix() const
{
    if (parent)
        return worldMatrix * parent->GetWorldMatrix();
    else
        return worldMatrix;
}

void ShiftEngine::ISceneNode::CreateWorldMatrix()
{
    MathLib::matrix<float, 4> scale;
    MathLib::matrix<float, 4> rotation;
    MathLib::matrix<float, 4> position;

    scale = MathLib::matrixScaling(Scale);
    rotation = Rotation.to_matrix();
    position = MathLib::matrixTranslation(Position);

    worldMatrix = scale * rotation * position;
}

MathLib::Vector3F ShiftEngine::ISceneNode::GetPosition() const
{
    if (parent)
        return parent->GetPosition() + Position;
    return Position;
}

void ShiftEngine::ISceneNode::SetPosition(const MathLib::Vector3F & val)
{
    Position = val;
    CreateWorldMatrix();
    if (pSceneGraph)
        pSceneGraph->MoveNodeCallback(this);
}

MathLib::Vector3F ShiftEngine::ISceneNode::GetScale() const
{
    if (parent)
        return parent->Scale + Scale;
    return Scale;
}

void ShiftEngine::ISceneNode::SetScale(const MathLib::Vector3F & val)
{
    Scale = val;
    CreateWorldMatrix();
    if (pSceneGraph)
        pSceneGraph->MoveNodeCallback(this);
}

void ShiftEngine::ISceneNode::SetScale(float val)
{
    SetScale(MathLib::Vector3F(val, val, val));
}

MathLib::qaFloat ShiftEngine::ISceneNode::GetRotation() const
{
    return Rotation;
}

void ShiftEngine::ISceneNode::SetRotation(const MathLib::qaFloat & val)
{
    Rotation = val;
}

void ShiftEngine::ISceneNode::RotateBy(const MathLib::qaFloat & val)
{
    Rotation = Rotation * val;
}

int ShiftEngine::ISceneNode::CheckVisibility(CameraSceneNode * activeCam) const
{
    MathLib::mat4f matWorld = GetWorldMatrix();
    MathLib::AABB bbox = GetBBox();

    MathLib::Vector4F vecMin = { bbox.bMin.x, bbox.bMin.y, bbox.bMin.z, 1.0f };
    MathLib::Vector4F vecMax = { bbox.bMax.x, bbox.bMax.y, bbox.bMax.z, 1.0f };
    vecMin = MathLib::vec4Transform(vecMin, matWorld);
    vecMax = MathLib::vec4Transform(vecMax, matWorld);
    MathLib::AABB newBbox(MathLib::Vector3F(vecMin.x, vecMin.y, vecMin.z), MathLib::Vector3F(vecMax.x, vecMax.y, vecMax.z));

    return activeCam->GetFrustumPtr()->CheckAABB(newBbox);
}

ShiftEngine::SceneGraph * ShiftEngine::ISceneNode::GetSceneGraph() const
{
    return pSceneGraph;
}

void ShiftEngine::ISceneNode::SetSceneGraph(SceneGraph * val)
{
    pSceneGraph = val;
}
