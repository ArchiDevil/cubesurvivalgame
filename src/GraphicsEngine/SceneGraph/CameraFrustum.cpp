#include "CameraFrustum.h"

enum planeEnum
{
    nearPlane,
    farPlane,
    leftPlane,
    rightPlane,
    topPlane,
    bottomPlane
};

void ShiftEngine::CameraFrustum::BuildFrustum(const MathLib::mat4f & matView, const MathLib::mat4f & matProj)
{
    memset(&planes, 0, sizeof(MathLib::plane<float>) * 6);

    const MathLib::mat4f matrix = matView * matProj;

    planes[nearPlane].a = matrix[0][3] + matrix[0][2];
    planes[nearPlane].b = matrix[1][3] + matrix[1][2];
    planes[nearPlane].c = matrix[2][3] + matrix[2][2];
    planes[nearPlane].d = matrix[3][3] + matrix[3][2];
    planes[nearPlane] = normalize(planes[nearPlane]);

    planes[farPlane].a = matrix[0][3] - matrix[0][2];
    planes[farPlane].b = matrix[1][3] - matrix[1][2];
    planes[farPlane].c = matrix[2][3] - matrix[2][2];
    planes[farPlane].d = matrix[3][3] - matrix[3][2];
    planes[farPlane] = normalize(planes[farPlane]);

    planes[leftPlane].a = matrix[0][3] + matrix[0][0];
    planes[leftPlane].b = matrix[1][3] + matrix[1][0];
    planes[leftPlane].c = matrix[2][3] + matrix[2][0];
    planes[leftPlane].d = matrix[3][3] + matrix[3][0];
    planes[leftPlane] = normalize(planes[leftPlane]);

    planes[rightPlane].a = matrix[0][3] - matrix[0][0];
    planes[rightPlane].b = matrix[1][3] - matrix[1][0];
    planes[rightPlane].c = matrix[2][3] - matrix[2][0];
    planes[rightPlane].d = matrix[3][3] - matrix[3][0];
    planes[rightPlane] = normalize(planes[rightPlane]);

    planes[topPlane].a = matrix[0][3] - matrix[0][1];
    planes[topPlane].b = matrix[1][3] - matrix[1][1];
    planes[topPlane].c = matrix[2][3] - matrix[2][1];
    planes[topPlane].d = matrix[3][3] - matrix[3][1];
    planes[topPlane] = normalize(planes[topPlane]);

    planes[bottomPlane].a = matrix[0][3] + matrix[0][1];
    planes[bottomPlane].b = matrix[1][3] + matrix[1][1];
    planes[bottomPlane].c = matrix[2][3] + matrix[2][1];
    planes[bottomPlane].d = matrix[3][3] + matrix[3][1];
    planes[bottomPlane] = normalize(planes[bottomPlane]);
}

ShiftEngine::CameraFrustum::CullingStatus ShiftEngine::CameraFrustum::CheckAABB(const MathLib::AABB & bbox) const
{
    MathLib::Vector3F vCorner[8];
    int totalIn = 0;

    // get the corners of the box into the vCorner array
    bbox.GetVertices(vCorner);

    // test all 8 corners against the 6 sides 
    // if all points are behind 1 specific plane, we are out
    // if we are in with all points, then we are fully in
    for (int plane = 0; plane < 6; ++plane)
    {
        int inCount = 8;
        int ptIn = 1;

        for (int vertex = 0; vertex < 8; ++vertex)
        {
            // test this point against the planes
            if (planeDotCoord(planes[plane], vCorner[vertex]) < 0)   //if point behind plane
            {
                ptIn = 0;
                --inCount;
            }
        }

        // were all the points outside of plane p?
        if (inCount == 0)
            return CS_Out;

        // check if they were all on the right side of the plane
        totalIn += ptIn;
    }

    // so if iTotalIn is 6, then all are inside the view
    if (totalIn == 6)
        return CS_In;

    // we must be partly in then otherwise
    return CS_Intersect;
}

ShiftEngine::CameraFrustum::CullingStatus ShiftEngine::CameraFrustum::CheckOOBB(const MathLib::OOBB & bbox) const
{
    MathLib::Vector3F vCorner[8];
    int totalIn = 0;

    bbox.GetVertices(vCorner);

    for (int plane = 0; plane < 6; ++plane)
    {
        int inCount = 8;
        int ptIn = 1;

        for (int vertex = 0; vertex < 8; ++vertex)
        {
            if (planeDotCoord(planes[plane], vCorner[vertex]) < 0)
            {
                ptIn = 0;
                --inCount;
            }
        }

        if (inCount == 0)
            return CS_Out;

        totalIn += ptIn;
    }

    if (totalIn == 6)
        return CS_In;

    return CS_Intersect;
}

ShiftEngine::CameraFrustum::CullingStatus ShiftEngine::CameraFrustum::CheckQTreeNode(const MathLib::AABB & bbox) const
{
    int totalIn = 0;
    MathLib::Vector3F buffer[4];
    buffer[0] = { bbox.bMin.x, bbox.bMin.y, 0.0f };
    buffer[1] = { bbox.bMin.x, bbox.bMax.y, 0.0f };
    buffer[2] = { bbox.bMax.x, bbox.bMin.y, 0.0f };
    buffer[3] = { bbox.bMax.x, bbox.bMax.y, 0.0f };

    for (int plane = 0; plane < 4; ++plane) //last 2 are top and bottom planes of frustum
    {
        int inCount = 4;
        int ptIn = 1;

        for (int vertex = 0; vertex < 4; ++vertex)
        {
            if (planeDotCoord(planes[plane], buffer[vertex]) < 0)
            {
                ptIn = 0;
                --inCount;
            }
        }

        if (inCount == 0)
            return CS_Out;

        totalIn += ptIn;
    }

    if (totalIn == 4)
        return CS_In;

    return CS_Intersect;
}
