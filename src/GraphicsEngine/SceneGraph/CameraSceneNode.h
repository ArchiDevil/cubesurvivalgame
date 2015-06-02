#pragma once

#include "ISceneNode.h"
#include "CameraFrustum.h"

#include <MathLib/math/quaternion.h>

#include <memory>

namespace ShiftEngine
{
    static const float MAX_ANGLE = 89.5f;

    class CameraSceneNode : public ISceneNode
    {
    public:
        CameraSceneNode();
        CameraSceneNode(MathLib::Vector3F position);

        void Initialize(float _screenWidth, float _screenHeight, float _zNear, float _zFar, float _FOV);
        void SetPosition(const Vector3F & pos);
        void Update();
        void MoveUpDown(float units);
        void MoveLeftRight(float units);
        void MoveForwardBackward(float units);
        void RotateByQuaternion(const MathLib::qaFloat & quat);
        void LookAt(const MathLib::Vector3F & point);
        void SetSphericalCoords(const MathLib::Vector3F & lookPoint, float phi, float theta, float r);

        MathLib::Vector3F GetLookVector() const;
        MathLib::Vector3F GetRightVector() const;
        MathLib::Vector3F GetPosition() const;
        MathLib::Vector3F GetUpVector() const;

        CameraFrustum * GetFrustumPtr();

        virtual void PushToRQ(RenderQueue & rq) override;

        const MathLib::mat4f & GetProjectionMatrix() const;
        const MathLib::mat4f & GetViewMatrix() const;

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

        MathLib::mat4f matView;
        MathLib::mat4f matProj;

        float zNear = 0.1f;
        float zFar = 100.0f;
        float fov = 60.0f;
        float screenWidth = 800.0f;
        float screenHeight = 600.0f;
        std::unique_ptr<CameraFrustum> frustum = nullptr;

        float viewAngle = 0.0f;
        MathLib::Vector3F angles = { 0.0f, 0.0f, 0.0f };
        MathLib::Vector3F upVector = { 0.0f, 0.0f, 1.0f };
        MathLib::Vector3F lookVector = { 0.0f, 1.0f, 0.0f };
        MathLib::Vector3F position = { 0.0f, 0.0f, 0.0f };
        MathLib::Vector3F rightVector = { 1.0f, 0.0f, 0.0f };

    };
}
