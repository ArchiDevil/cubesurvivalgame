#pragma once

#include <MathLib/math.h>

namespace ShiftEngine
{
    class CameraFrustum
    {
    public:
        enum CullingStatus
        {
            CS_Out,
            CS_Intersect,
            CS_In
        };

        void BuildFrustum(const MathLib::mat4f & matView, const MathLib::mat4f & matProj);

        CullingStatus CheckAABB(const MathLib::AABB & bbox) const;
        CullingStatus CheckOOBB(const MathLib::OOBB & bbox) const;
        CullingStatus CheckQTreeNode(const MathLib::AABB & bbox) const;

        //not implemented
        bool CheckPoint(const MathLib::Vector3F & point) const;
        bool CheckBox(const MathLib::Vector3F & center, const MathLib::Vector3F & radius) const;
        bool CheckSphere(const MathLib::Vector3F & position, const MathLib::Vector3F & radius) const;
        bool CheckRectangle(const MathLib::Vector3F & position, const MathLib::Vector3F & sizes) const;

    private:
        MathLib::plane<float> planes[6]; //6 плоскостей - передн€€, задн€€, 2 боковые, 2 верхн€€/нижн€€
    };
}
