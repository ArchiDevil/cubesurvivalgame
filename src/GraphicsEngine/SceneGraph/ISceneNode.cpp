#include "ISceneNode.h"

#include "CameraSceneNode.h"
#include "SceneGraph.h"

#include <algorithm>
#include <iostream>

ShiftEngine::ISceneNode::ISceneNode( const MathLib::AABB & _bbox )
    : Scale(1.0f, 1.0f, 1.0f), Rotation(0.0f, 0.0f, 0.0f),
      Position(0.0f, 0.0f, 0.0f), parent(nullptr), bbox(_bbox),
	  pSceneGraph(nullptr)
{
}

ShiftEngine::ISceneNode::~ISceneNode()
{
    for(auto iter = children.begin();
            iter != children.end(); iter++)
    {
        (*iter)->release();
    }
}

void ShiftEngine::ISceneNode::AddChild(ISceneNode * node)
{
    //TODO: fix memleak here
    if(!node)
        return;

	//every node must have only 1 parent

    node->addRef();	//to avoid killing by parent

    if(node->parent)
        node->parent->RemoveChild(node);

    node->parent = this;
	//find empty place
	for(auto & elem : children)
	{
		if(elem == nullptr)
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
	if(iter != children.end())
	{
		*iter = nullptr;
		*iter = *(--children.end());
		children.pop_back();
	}
}

void ShiftEngine::ISceneNode::SetParent(ISceneNode * _parent)
{
    if(_parent)
        _parent->AddChild(this);	//it will set parent to node
    else
        parent = _parent;
}

void ShiftEngine::ISceneNode::RemoveParent()
{
    if(this->parent)
        parent->RemoveChild(this);
    parent = nullptr;
}

void ShiftEngine::ISceneNode::KillSelf()
{
    if(this->parent)
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

void ShiftEngine::ISceneNode::Draw( RenderQueue & rq )
{
    this->PushToRQ(rq);

    if(!children.empty())
    {
        for (auto iter = children.begin();
                iter != children.end();
                ++iter)
        {
            (*iter)->Draw(rq);
        }
    }
}

D3DXMATRIX ShiftEngine::ISceneNode::GetWorldMatrix() const
{
    if(parent)
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
	D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
	D3DXMatrixTranslation(&pos, Position.x, Position.y, Position.z);
	worldMatrix = scale * rot * pos;
}

Vector3F ShiftEngine::ISceneNode::GetPosition() const
{
    if(parent)
        return parent->GetPosition() + Position;
    return Position;
}

void ShiftEngine::ISceneNode::SetPosition( const Vector3F & val )
{
    Position = val;
	CreateWorldMatrix();
	if(pSceneGraph)
		pSceneGraph->MoveNodeCallback(this);
}

Vector3F ShiftEngine::ISceneNode::GetScale() const
{
    if(parent)
        return parent->Scale + Scale;
    return Scale;
}

void ShiftEngine::ISceneNode::SetScale( const Vector3F & val )
{
    Scale = val;
	CreateWorldMatrix();
	if(pSceneGraph)
		pSceneGraph->MoveNodeCallback(this);
}

Vector3F ShiftEngine::ISceneNode::GetRotation() const
{
    if(parent)
        return parent->Rotation + Rotation;
    return Rotation;
}

void ShiftEngine::ISceneNode::SetRotation( const Vector3F & val )
{
	Rotation = val;
	CreateWorldMatrix();
	if(pSceneGraph)
		pSceneGraph->MoveNodeCallback(this);
}

MathLib::AABB ShiftEngine::ISceneNode::GetBBox() const
{
    D3DXMATRIX matWorld = this->GetWorldMatrix();
    D3DXVECTOR4 points[8];
    points[0] = D3DXVECTOR4(bbox.bMin.x, bbox.bMin.y, bbox.bMin.z, 1.0f);
    points[1] = D3DXVECTOR4(bbox.bMin.x, bbox.bMin.y, bbox.bMax.z, 1.0f);
    points[2] = D3DXVECTOR4(bbox.bMin.x, bbox.bMax.y, bbox.bMin.z, 1.0f);
    points[3] = D3DXVECTOR4(bbox.bMax.x, bbox.bMin.y, bbox.bMin.z, 1.0f);
    points[4] = D3DXVECTOR4(bbox.bMax.x, bbox.bMax.y, bbox.bMin.z, 1.0f);
    points[5] = D3DXVECTOR4(bbox.bMin.x, bbox.bMax.y, bbox.bMax.z, 1.0f);
    points[6] = D3DXVECTOR4(bbox.bMax.x, bbox.bMin.y, bbox.bMax.z, 1.0f);
    points[7] = D3DXVECTOR4(bbox.bMax.x, bbox.bMax.y, bbox.bMax.z, 1.0f);

    D3DXVECTOR3 min, max;

    for (int i = 0; i < 8 ; i++)
        D3DXVec4Transform(points + i, points + i, &matWorld);

    min.x = points[0].x;
    min.y = points[0].y;
    min.z = points[0].z;
    max.x = points[0].x;
    max.y = points[0].y;
    max.z = points[0].z;
    for (int i = 0; i < 8 ; i++)
    {
        if(points[i].x < min.x)
            min.x = points[i].x;
        if(points[i].y < min.y)
            min.y = points[i].y;
        if(points[i].z < min.z)
            min.z = points[i].z;

        if(points[i].x > max.x)
            max.x = points[i].x;
        if(points[i].y > max.y)
            max.y = points[i].y;
        if(points[i].z > max.z)
            max.z = points[i].z;
    }

    MathLib::AABB newBbox(Vector3F(min.x, min.y, min.z), Vector3F(max.x, max.y, max.z));
    return newBbox;
}

int ShiftEngine::ISceneNode::CheckVisibility(CameraSceneNode * activeCam) const
{
    D3DXMATRIX matWorld = this->GetWorldMatrix();
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

void ShiftEngine::ISceneNode::SetSceneGraph( SceneGraph * val )
{
	pSceneGraph = val;
}
