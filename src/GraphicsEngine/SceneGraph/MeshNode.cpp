#include "MeshNode.h"

#include "../ShiftEngine.h"

ShiftEngine::MeshNode::MeshNode(const MeshDataPtr & _data, const Material * mat, const MathLib::AABB & _bbox)
    : ISceneNode()
    , bbox(_bbox)
    , isVisible(true)
    , material(*mat)
    , Data(_data)
{
}

ShiftEngine::MeshNode::~MeshNode()
{
}

void ShiftEngine::MeshNode::PushToRQ(RenderQueue & rq)
{
    if (this->IsVisible())// && this->CheckVisibility(rq.GetActiveCamera()))
        rq.AddRenderableNode(this);
}

bool ShiftEngine::MeshNode::IsVisible() const
{
    return isVisible;
}

void ShiftEngine::MeshNode::SetVisibility(bool vis)
{
    isVisible = vis;
}

ShiftEngine::Material * ShiftEngine::MeshNode::GetMaterialPtr()
{
    return &material;
}

void ShiftEngine::MeshNode::SetMaterial(const ShiftEngine::Material * val)
{
    material = Material(*val);
}

ShiftEngine::MeshDataPtr ShiftEngine::MeshNode::GetDataPtr() const
{
    return Data;
}

void ShiftEngine::MeshNode::SetDataPtr(MeshDataPtr data)
{
    Data = data;
}

int ShiftEngine::MeshNode::Render()
{
    if (!Data)
        return 0;
    return GetContextManager()->DrawMesh(Data);
}

MathLib::AABB ShiftEngine::MeshNode::GetBBox() const
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

    for (int i = 0; i < 8; i++)
        D3DXVec4Transform(points + i, points + i, &matWorld);

    min.x = points[0].x;
    min.y = points[0].y;
    min.z = points[0].z;
    max.x = points[0].x;
    max.y = points[0].y;
    max.z = points[0].z;
    for (int i = 0; i < 8; i++)
    {
        if (points[i].x < min.x)
            min.x = points[i].x;
        if (points[i].y < min.y)
            min.y = points[i].y;
        if (points[i].z < min.z)
            min.z = points[i].z;

        if (points[i].x > max.x)
            max.x = points[i].x;
        if (points[i].y > max.y)
            max.y = points[i].y;
        if (points[i].z > max.z)
            max.z = points[i].z;
    }

    MathLib::AABB newBbox(Vector3F(min.x, min.y, min.z), Vector3F(max.x, max.y, max.z));
    return newBbox;
}

void ShiftEngine::MeshNode::SetBBox(const MathLib::AABB &bbox)
{
    this->bbox = bbox;
}
