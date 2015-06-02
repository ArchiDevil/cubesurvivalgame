#include "CameraSceneNode.h"

#include "../RenderQueue.h"

ShiftEngine::CameraSceneNode::CameraSceneNode()
    : ISceneNode()
{
}

ShiftEngine::CameraSceneNode::CameraSceneNode(MathLib::Vector3F position)
    : ISceneNode()
    , position(position)
{
}

void ShiftEngine::CameraSceneNode::Initialize(float _screenWidth, float _screenHeight, float _zNear, float _zFar, float _FOV)
{
    zNear = _zNear;
    zFar = _zFar;
    fov = _FOV;
    screenWidth = _screenWidth;
    screenHeight = _screenHeight;

    Vector3F LOOK_POS = lookVector + position;
    matView = MathLib::matrixLookAtRH<float, 4>(position, LOOK_POS, upVector);
    RebuildProjMatrix();

    frustum.reset(new CameraFrustum());
}

void ShiftEngine::CameraSceneNode::SetPosition(const Vector3F & pos)
{
    position.x = pos.x;
    position.y = pos.y;
    position.z = pos.z;
}

void ShiftEngine::CameraSceneNode::MoveUpDown(float units)
{
    position += upVector * units;
}

void ShiftEngine::CameraSceneNode::MoveLeftRight(float units)
{
    position += rightVector * units;
}

void ShiftEngine::CameraSceneNode::MoveForwardBackward(float units)
{
    position += lookVector * units;
}

void ShiftEngine::CameraSceneNode::Update()
{
    Vector3F eye = lookVector + position;
    matView = MathLib::matrixLookAtRH<float, 4>(position, eye, upVector);
    frustum->BuildFrustum(matView, matProj);
}

MathLib::Vector3F ShiftEngine::CameraSceneNode::GetLookVector() const
{
    return lookVector;
}

MathLib::Vector3F ShiftEngine::CameraSceneNode::GetRightVector() const
{
    return rightVector;
}

MathLib::Vector3F ShiftEngine::CameraSceneNode::GetPosition() const
{
    return position;
}

ShiftEngine::CameraFrustum * ShiftEngine::CameraSceneNode::GetFrustumPtr()
{
    return frustum.get();
}

void ShiftEngine::CameraSceneNode::LookAt(const MathLib::Vector3F & point)
{
    lookVector = MathLib::normalize(point);
}

void ShiftEngine::CameraSceneNode::PushToRQ(RenderQueue & /*rq*/)
{
}

const MathLib::mat4f & ShiftEngine::CameraSceneNode::GetProjectionMatrix() const
{
    return matProj;
}

const MathLib::mat4f & ShiftEngine::CameraSceneNode::GetViewMatrix() const
{
    return matView;
}

MathLib::Vector3F ShiftEngine::CameraSceneNode::GetUpVector() const
{
    return upVector;
}

float ShiftEngine::CameraSceneNode::GetZNear() const
{
    return zNear;
}

float ShiftEngine::CameraSceneNode::GetZFar() const
{
    return zFar;
}

float ShiftEngine::CameraSceneNode::GetFOV() const
{
    return fov;
}

void ShiftEngine::CameraSceneNode::SetZFar(float val)
{
    zFar = val;
    RebuildProjMatrix();
}

void ShiftEngine::CameraSceneNode::SetZNear(float val)
{
    zNear = val;
    RebuildProjMatrix();
}

void ShiftEngine::CameraSceneNode::SetFOV(float val)
{
    fov = val;
    RebuildProjMatrix();
}

void ShiftEngine::CameraSceneNode::SetScreenWidth(float val)
{
    screenWidth = val;
    RebuildProjMatrix();
}

void ShiftEngine::CameraSceneNode::SetScreenHeight(float val)
{
    screenHeight = val;
    RebuildProjMatrix();
}

void ShiftEngine::CameraSceneNode::RebuildProjMatrix()
{
    matProj = MathLib::matrixPerspectiveFovRH<float, 4>(M_PIF * fov / 180.0f,       // vertical FoV
                                                        screenWidth / screenHeight, // screen rate
                                                        zNear,
                                                        zFar);
}

void ShiftEngine::CameraSceneNode::RotateByQuaternion(const MathLib::qaFloat & quat)
{
    // transform all vectors
    Vector3F look(lookVector.x, lookVector.y, lookVector.z);
    look = look * quat;
    lookVector = look;

    Vector3F up(upVector.x, upVector.y, upVector.z);
    up = up * quat;
    upVector = up;

    Vector3F right(rightVector.x, rightVector.y, rightVector.z);
    right = right * quat;
    rightVector = right;

    Vector3F LOOK_POS = lookVector + position;
    matView = MathLib::matrixLookAtRH<float, 4>(position, LOOK_POS, upVector);
}

void ShiftEngine::CameraSceneNode::SetSphericalCoords(const Vector3F & center, float phi, float theta, float r)
{
    position = MathLib::GetPointOnSphere(center, r, phi, theta);
    lookVector = center - position;
    lookVector = normalize(lookVector);
    // we assume that UP vector is always direct to up
    upVector = { 0.0f, 0.0f, 1.0f };
    rightVector = MathLib::cross(lookVector, upVector);
    rightVector = normalize(rightVector);
    upVector = MathLib::cross(rightVector, lookVector);
    upVector = normalize(upVector);

    Vector3F at = lookVector + position;
    matView = MathLib::matrixLookAtRH<float, 4>(position, at, upVector);
}

MathLib::AABB ShiftEngine::CameraSceneNode::GetBBox() const
{
    return MathLib::AABB();
}
