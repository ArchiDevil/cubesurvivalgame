#include "CameraFrustum.h"

void ShiftEngine::CameraFrustum::BuildFrustum(const MathLib::mat4f & matView, const MathLib::mat4f & matProj)
{
    memset(&planes, 0, sizeof(MathLib::plane<float>) * 6);

    const MathLib::mat4f matrix = matView * matProj;

    // Вычисление плоскостей
    planes[0].a = matrix[0][3] + matrix[0][2]; // Задняя плоскость
    planes[0].b = matrix[1][3] + matrix[1][2];
    planes[0].c = matrix[2][3] + matrix[2][2];
    planes[0].d = matrix[3][3] + matrix[3][2];
    planes[0] = normalize(planes[0]);

    planes[1].a = matrix[0][3] - matrix[0][2]; // Передняя плоскость
    planes[1].b = matrix[1][3] - matrix[1][2];
    planes[1].c = matrix[2][3] - matrix[2][2];
    planes[1].d = matrix[3][3] - matrix[3][2];
    planes[1] = normalize(planes[1]);

    planes[2].a = matrix[0][3] + matrix[0][0]; // Левая плоскость
    planes[2].b = matrix[1][3] + matrix[1][0];
    planes[2].c = matrix[2][3] + matrix[2][0];
    planes[2].d = matrix[3][3] + matrix[3][0];
    planes[2] = normalize(planes[2]);

    planes[3].a = matrix[0][3] - matrix[0][0]; // Правая плоскость
    planes[3].b = matrix[1][3] - matrix[1][0];
    planes[3].c = matrix[2][3] - matrix[2][0];
    planes[3].d = matrix[3][3] - matrix[3][0];
    planes[3] = normalize(planes[3]);

    planes[4].a = matrix[0][3] - matrix[0][1]; // Верхняя плоскость
    planes[4].b = matrix[1][3] - matrix[1][1];
    planes[4].c = matrix[2][3] - matrix[2][1];
    planes[4].d = matrix[3][3] - matrix[3][1];
    planes[4] = normalize(planes[4]);

    planes[5].a = matrix[0][3] + matrix[0][1]; // Нижняя плоскость
    planes[5].b = matrix[1][3] + matrix[1][1];
    planes[5].c = matrix[2][3] + matrix[2][1];
    planes[5].d = matrix[3][3] + matrix[3][1];
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
