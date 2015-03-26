#pragma once

#include <D3DX10.h>

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

        CameraFrustum();
        ~CameraFrustum();

        void BuildFrustum(D3DXMATRIX * matView, D3DXMATRIX * matProj);

        CullingStatus CheckAABB(const MathLib::AABB & bbox) const;
        CullingStatus CheckOOBB(const MathLib::OOBB & bbox) const;
        CullingStatus CheckQTreeNode(const MathLib::AABB & bbox) const;

        //not implemented
        bool CheckPoint(const D3DXVECTOR3 & point) const;
        bool CheckBox(const D3DXVECTOR3 & center, const D3DXVECTOR3 & radius) const;
        bool CheckSphere(const D3DXVECTOR3 & position, const D3DXVECTOR3 & radius) const;
        bool CheckRectangle(const D3DXVECTOR3 & position, const D3DXVECTOR3 & sizes) const;
    private:
        D3DXPLANE planes[6]; //6 плоскостей - передн€€, задн€€, 2 боковые, 2 верхн€€/нижн€€
    };
}
