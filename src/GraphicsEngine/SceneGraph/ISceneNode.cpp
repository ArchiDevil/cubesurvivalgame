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

D3DXMATRIX ShiftEngine::ISceneNode::GetWorldMatrix() const
{
    if (parent)
        return worldMatrix * parent->GetWorldMatrix();
    else
        return worldMatrix;
}

void ShiftEngine::ISceneNode::CreateWorldMatrix()
{
    D3DXMATRIX scale;
    D3DXMATRIX rot;
    D3DXMATRIX pos;
    D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
    D3DXQUATERNION q;
    q.x = Rotation.vector.x;
    q.y = Rotation.vector.y;
    q.z = Rotation.vector.z;
    q.w = Rotation.w;
    //auto matrix = Rotation.to_matrix();
    D3DXMatrixRotationQuaternion(&rot, &q);
    D3DXMatrixTranslation(&pos, Position.x, Position.y, Position.z);
    worldMatrix = scale * rot * pos;
}

Vector3F ShiftEngine::ISceneNode::GetPosition() const
{
    if (parent)
        return parent->GetPosition() + Position;
    return Position;
}

void ShiftEngine::ISceneNode::SetPosition(const Vector3F & val)
{
    Position = val;
    CreateWorldMatrix();
    if (pSceneGraph)
        pSceneGraph->MoveNodeCallback(this);
}

Vector3F ShiftEngine::ISceneNode::GetScale() const
{
    if (parent)
        return parent->Scale + Scale;
    return Scale;
}

void ShiftEngine::ISceneNode::SetScale(const Vector3F & val)
{
    Scale = val;
    CreateWorldMatrix();
    if (pSceneGraph)
        pSceneGraph->MoveNodeCallback(this);
}

void ShiftEngine::ISceneNode::SetScale(float val)
{
    SetScale(Vector3F(val, val, val));
}

qaFloat ShiftEngine::ISceneNode::GetRotation() const
{
    return Rotation;
}

void ShiftEngine::ISceneNode::SetRotation(const qaFloat & val)
{
    Rotation = val;
}

void ShiftEngine::ISceneNode::RotateBy(const qaFloat & val)
{
    Rotation = Rotation * val;
}

int ShiftEngine::ISceneNode::CheckVisibility(CameraSceneNode * activeCam) const
{
    D3DXMATRIX matWorld = this->GetWorldMatrix();
    MathLib::AABB bbox = GetBBox();
    D3DXVECTOR4 vecMin = D3DXVECTOR4(bbox.bMin.x, bbox.bMin.y, bbox.bMin.z, 1.0f);
    D3DXVECTOR4 vecMax = D3DXVECTOR4(bbox.bMax.x, bbox.bMax.y, bbox.bMax.z, 1.0f);
    D3DXVec4Transform(&vecMin, &vecMin, &matWorld);
    D3DXVec4Transform(&vecMax, &vecMax, &matWorld);
    MathLib::AABB newBbox(MathLib::Vector3F(vecMin.x, vecMin.y, vecMin.z), Vector3F(vecMax.x, vecMax.y, vecMax.z));

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
