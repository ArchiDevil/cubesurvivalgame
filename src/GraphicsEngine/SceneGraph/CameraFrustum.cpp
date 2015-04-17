#include "CameraFrustum.h"

#if defined(D3D10_RENDER)
#   include <D3DX10.h>
#elif defined(D3D11_RENDER)
#   include <D3DX11.h>
#endif // D3D10_RENDER

void ShiftEngine::CameraFrustum::BuildFrustum(D3DXMATRIX * matView, D3DXMATRIX * matProj)
{
    D3DXMATRIX Matrix;

    ::ZeroMemory(&planes, sizeof(MathLib::plane<float>) * 6);
    ::ZeroMemory(&Matrix, sizeof(D3DXMATRIX));

    D3DXMatrixMultiply(&Matrix, matView, matProj);

    // Вычисление плоскостей
    planes[0].a = Matrix._14 + Matrix._13; // Задняя плоскость
    planes[0].b = Matrix._24 + Matrix._23;
    planes[0].c = Matrix._34 + Matrix._33;
    planes[0].d = Matrix._44 + Matrix._43;
    planes[0] = normalize(planes[0]);

    planes[1].a = Matrix._14 - Matrix._13; // Передняя плоскость
    planes[1].b = Matrix._24 - Matrix._23;
    planes[1].c = Matrix._34 - Matrix._33;
    planes[1].d = Matrix._44 - Matrix._43;
    planes[1] = normalize(planes[1]);

    planes[2].a = Matrix._14 + Matrix._11; // Левая плоскость
    planes[2].b = Matrix._24 + Matrix._21;
    planes[2].c = Matrix._34 + Matrix._31;
    planes[2].d = Matrix._44 + Matrix._41;
    planes[2] = normalize(planes[2]);

    planes[3].a = Matrix._14 - Matrix._11; // Правая плоскость
    planes[3].b = Matrix._24 - Matrix._21;
    planes[3].c = Matrix._34 - Matrix._31;
    planes[3].d = Matrix._44 - Matrix._41;
    planes[3] = normalize(planes[3]);

    planes[4].a = Matrix._14 - Matrix._12; // Верхняя плоскость
    planes[4].b = Matrix._24 - Matrix._22;
    planes[4].c = Matrix._34 - Matrix._32;
    planes[4].d = Matrix._44 - Matrix._42;
    planes[4] = normalize(planes[4]);

    planes[5].a = Matrix._14 + Matrix._12; // Нижняя плоскость
    planes[5].b = Matrix._24 + Matrix._22;
    planes[5].c = Matrix._34 + Matrix._32;
    planes[5].d = Matrix._44 + Matrix._42;
    planes[5] = normalize(planes[5]);
}

ShiftEngine::CameraFrustum::CullingStatus ShiftEngine::CameraFrustum::CheckAABB(const MathLib::AABB & bbox) const
{
    MathLib::Vector3F vCorner[8];
    int iTotalIn = 0;

    // get the corners of the box into the vCorner array
    bbox.GetVertices(vCorner);

    // test all 8 corners against the 6 sides 
    // if all points are behind 1 specific plane, we are out
    // if we are in with all points, then we are fully in
    for (int p = 0; p < 6; ++p)
    {

        int iInCount = 8;
        int iPtIn = 1;

        for (int i = 0; i < 8; ++i)
        {
            // test this point against the planes
            if (planeDotCoord(planes[p], vCorner[i]) < 0)   //if point behind plane
            {
                iPtIn = 0;
                --iInCount;
            }
        }

        // were all the points outside of plane p?
        if (iInCount == 0)
            return CS_Out;

        // check if they were all on the right side of the plane
        iTotalIn += iPtIn;
    }

    // so if iTotalIn is 6, then all are inside the view
    if (iTotalIn == 6)
        return CS_In;

    // we must be partly in then otherwise
    return CS_Intersect;
}

ShiftEngine::CameraFrustum::CullingStatus ShiftEngine::CameraFrustum::CheckOOBB(const MathLib::OOBB & bbox) const
{
    MathLib::Vector3F vCorner[8];
    int iTotalIn = 0;

    bbox.GetVertices(vCorner);

    for (int p = 0; p < 6; ++p)
    {
        int iInCount = 8;
        int iPtIn = 1;

        for (int i = 0; i < 8; ++i)
        {
            if (planeDotCoord(planes[p], vCorner[i]) < 0)
            {
                iPtIn = 0;
                --iInCount;
            }
        }

        if (iInCount == 0)
            return CS_Out;

        iTotalIn += iPtIn;
    }

    if (iTotalIn == 6)
        return CS_In;

    return CS_Intersect;
}

ShiftEngine::CameraFrustum::CullingStatus ShiftEngine::CameraFrustum::CheckQTreeNode(const MathLib::AABB & bbox) const
{
    int iTotalIn = 0;
    MathLib::Vector3F buffer[4];
    buffer[0] = { bbox.bMin.x, bbox.bMin.y, 0.0f };
    buffer[1] = { bbox.bMin.x, bbox.bMax.y, 0.0f };
    buffer[2] = { bbox.bMax.x, bbox.bMin.y, 0.0f };
    buffer[3] = { bbox.bMax.x, bbox.bMax.y, 0.0f };

    for (int p = 0; p < 4; ++p) //last 2 is a top and bottom planes of frustum
    {
        int iInCount = 4;
        int iPtIn = 1;

        for (int i = 0; i < 4; ++i)
        {
            if (planeDotCoord(planes[p], buffer[i]) < 0)
            {
                iPtIn = 0;
                --iInCount;
            }
        }

        if (iInCount == 0)
            return CS_Out;

        iTotalIn += iPtIn;
    }

    if (iTotalIn == 4)
        return CS_In;

    return CS_Intersect;
}
