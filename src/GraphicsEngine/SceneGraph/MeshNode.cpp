#include "MeshNode.h"

#include "../ShiftEngine.h"

ShiftEngine::MeshNode::MeshNode(const IMeshDataPtr & _data, const Material * mat, const MathLib::AABB & _bbox)
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
    int visibility = 1;//CheckVisibility(rq.GetActiveCamera());

    if (IsVisible() && visibility)
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

ShiftEngine::IMeshDataPtr ShiftEngine::MeshNode::GetDataPtr() const
{
    return Data;
}

void ShiftEngine::MeshNode::SetDataPtr(IMeshDataPtr data)
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
    MathLib::mat4f matWorld = GetWorldMatrix();
    MathLib::Vector4F points[8];
    points[0] = { bbox.bMin.x, bbox.bMin.y, bbox.bMin.z, 1.0f };
    points[1] = { bbox.bMin.x, bbox.bMin.y, bbox.bMax.z, 1.0f };
    points[2] = { bbox.bMin.x, bbox.bMax.y, bbox.bMin.z, 1.0f };
    points[3] = { bbox.bMax.x, bbox.bMin.y, bbox.bMin.z, 1.0f };
    points[4] = { bbox.bMax.x, bbox.bMax.y, bbox.bMin.z, 1.0f };
    points[5] = { bbox.bMin.x, bbox.bMax.y, bbox.bMax.z, 1.0f };
    points[6] = { bbox.bMax.x, bbox.bMin.y, bbox.bMax.z, 1.0f };
    points[7] = { bbox.bMax.x, bbox.bMax.y, bbox.bMax.z, 1.0f };

    MathLib::Vector3F min, max;

    for (int i = 0; i < 8; i++)
        points[i] = MathLib::vec4Transform(points[i], matWorld);

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
