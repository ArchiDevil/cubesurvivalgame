#pragma once

#include "AABB.h"
#include "matrix.h"

namespace MathLib
{
    class OOBB
    {
    public:
        OOBB(Vector3F _position = Vector3F(), Vector3F _size = Vector3F(1.0f, 1.0f, 1.0f), Vector3F _angles = Vector3F());
        void GetVertices(Vector3F vertices[8]) const;
        AABB GetAABB() const;

        bool IntersectsAABB(const AABB & bbox);
        bool IntersectsOOBB(const OOBB & bbox);
        bool IntersectsSphere(const Vector3F & center, float raduis);
        bool IntersectsLineSegment(const Vector3F &mid, const Vector3F &dir, const int hl);
        bool IntersectsTriangle(const Vector3F &pnt1, const Vector3F &pnt2, const Vector3F &pnt3);

        Vector3F position;
        Vector3F size;
        matrix<float, 3> rotation;
    };
}
