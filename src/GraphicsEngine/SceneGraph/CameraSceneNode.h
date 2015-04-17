#pragma once

#include "ISceneNode.h"
#include "CameraFrustum.h"

#include <MathLib/math/quaternion.h>

#include <memory>

//REMOVE D3DX LIB !!!

namespace ShiftEngine
{
    static const float MAX_ANGLE = 89.5f;

    class CameraSceneNode : public ISceneNode
    {
    public:
        CameraSceneNode();
        CameraSceneNode(D3DXVECTOR3 _pos, D3DXVECTOR3 _up, D3DXVECTOR3 _right);

        void Initialize(float _screenWidth, float _screenHeight, float _zNear, float _zFar, float _FOV);

        void SetPosition(float x, float y, float z);
        void SetPosition(const Vector3F & pos);

        void Update();

        void MoveUpDown(float units);
        void MoveLeftRight(float units);
        void MoveForwardBackward(float units);
        void RotateByQuaternion(const MathLib::qaFloat & quat);
        void LookAt(D3DXVECTOR3 point);
        void SetSphericalCoords(const D3DXVECTOR3 & lookPoint, float phi, float theta, float r);

        D3DXVECTOR3 GetLookVector() const;
        D3DXVECTOR3 GetRightVector() const;
        D3DXVECTOR3 GetPosition() const;
        D3DXVECTOR3 GetUpVector() const;

        CameraFrustum * GetFrustumPtr();

        virtual void PushToRQ(RenderQueue & rq) override;

        const D3DXMATRIX & GetProjectionMatrix() const;
        const D3DXMATRIX & GetViewMatrix() const;

        void SetZNear(float val);
        float GetZNear() const;

        void SetZFar(float val);
        float GetZFar() const;

        void SetFOV(float val);
        float GetFOV() const;

        void SetScreenWidth(float val);
        void SetScreenHeight(float val);

        virtual MathLib::AABB GetBBox() const override;

    private:
        void RebuildProjMatrix();

        D3DXMATRIX matView;
        D3DXMATRIX matProj;

        float zNear = 0.1f;
        float zFar = 100.0f;
        float FOV = 60.0f;
        float screenWidth = 800.0f;
        float screenHeight = 600.0f;
        std::unique_ptr<CameraFrustum> Frustum = nullptr;

        float ViewAngle = 0.0f;
        D3DXVECTOR3 Angles = { 0.0f, 0.0f, 0.0f };

        D3DXVECTOR3 UP = { 0.0f, 0.0f, 1.0f };
        D3DXVECTOR3 LOOK = { 0.0f, 1.0f, 0.0f };
        D3DXVECTOR3 POS = { 0.0f, 0.0f, 0.0f };
        D3DXVECTOR3 RIGHT = { 1.0f, 0.0f, 0.0f };

    };
}
