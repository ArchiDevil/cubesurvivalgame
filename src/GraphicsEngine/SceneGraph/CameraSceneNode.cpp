#include "CameraSceneNode.h"

#include "../RenderQueue.h"

ShiftEngine::CameraSceneNode::CameraSceneNode()
    : ISceneNode()
{
}

ShiftEngine::CameraSceneNode::CameraSceneNode(D3DXVECTOR3 pos, D3DXVECTOR3 up, D3DXVECTOR3 right)
    : ISceneNode()
    , UP(up)
    , POS(pos)
    , RIGHT(right)
{
}

void ShiftEngine::CameraSceneNode::Initialize(float _screenWidth, float _screenHeight, float _zNear, float _zFar, float _FOV)
{
    zNear = _zNear;
    zFar = _zFar;
    FOV = _FOV;
    screenWidth = _screenWidth;
    screenHeight = _screenHeight;

    D3DXVECTOR3 LOOK_POS = LOOK + POS;
    D3DXMatrixLookAtRH(&matView, &POS, &LOOK_POS, &UP);
    RebuildProjMatrix();

    Frustum.reset(new CameraFrustum());
}

void ShiftEngine::CameraSceneNode::SetPosition(float x, float y, float z)
{
    POS.x = x;
    POS.y = y;
    POS.z = z;
}

void ShiftEngine::CameraSceneNode::SetPosition(const Vector3F & pos)
{
    POS.x = pos.x;
    POS.y = pos.y;
    POS.z = pos.z;
}

void ShiftEngine::CameraSceneNode::MoveUpDown(float units)
{
    POS += UP * units;
}

void ShiftEngine::CameraSceneNode::MoveLeftRight(float units)
{
    POS += RIGHT * units;
}

void ShiftEngine::CameraSceneNode::MoveForwardBackward(float units)
{
    POS += LOOK * units;
}

void ShiftEngine::CameraSceneNode::Update()
{
    D3DXVECTOR3 LOOK_POS = LOOK + POS;
    D3DXMatrixLookAtRH(&matView, &POS, &LOOK_POS, &UP);
    Frustum->BuildFrustum(&matView, &matProj);
}

D3DXVECTOR3 ShiftEngine::CameraSceneNode::GetLookVector() const
{
    return LOOK;
}

D3DXVECTOR3 ShiftEngine::CameraSceneNode::GetRightVector() const
{
    return RIGHT;
}

D3DXVECTOR3 ShiftEngine::CameraSceneNode::GetPosition() const
{
    return POS;
}

ShiftEngine::CameraFrustum * ShiftEngine::CameraSceneNode::GetFrustumPtr()
{
    return Frustum.get();
}

void ShiftEngine::CameraSceneNode::LookAt(D3DXVECTOR3 point)
{
    LOOK = point;
    D3DXVec3Normalize(&LOOK, &point);
}

void ShiftEngine::CameraSceneNode::PushToRQ(RenderQueue & /*rq*/)
{
}

const D3DXMATRIX & ShiftEngine::CameraSceneNode::GetProjectionMatrix() const
{
    return matProj;
}

const D3DXMATRIX & ShiftEngine::CameraSceneNode::GetViewMatrix() const
{
    return matView;
}

D3DXVECTOR3 ShiftEngine::CameraSceneNode::GetUpVector() const
{
    return UP;
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
    return FOV;
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
    FOV = val;
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
    D3DXMatrixPerspectiveFovRH(
        &matProj,                   // out
        (float)D3DX_PI * FOV / 180, // vertical FoV
        screenWidth / screenHeight, // screen rate
        zNear,                      // near
        zFar);                      // far
}

void ShiftEngine::CameraSceneNode::RotateByQuaternion(const MathLib::qaFloat & quat)
{
    // transform all vectors
    Vector3F look(LOOK.x, LOOK.y, LOOK.z);
    look = look * quat;
    LOOK = D3DXVECTOR3(look.x, look.y, look.z);

    Vector3F up(UP.x, UP.y, UP.z);
    up = up * quat;
    UP = D3DXVECTOR3(up.x, up.y, up.z);

    Vector3F right(RIGHT.x, RIGHT.y, RIGHT.z);
    right = right * quat;
    RIGHT = D3DXVECTOR3(right.x, right.y, right.z);

    D3DXVECTOR3 LOOK_POS = LOOK + POS;
    D3DXMatrixLookAtRH(&matView, &POS, &LOOK_POS, &UP);
}

void ShiftEngine::CameraSceneNode::SetSphericalCoords(const D3DXVECTOR3 & lookPoint, float phi, float theta, float r)
{
    float x = r * sin(theta * 0.0175f) * cos(phi * 0.0175f);
    float y = r * sin(theta * 0.0175f) * sin(phi * 0.0175f);
    float z = r * cos(theta * 0.0175f);

    POS = D3DXVECTOR3(x, y, z) + lookPoint;
    LOOK = lookPoint - POS;
    D3DXVec3Normalize(&LOOK, &LOOK);
    // we assume that UP vector is always direct to up
    UP = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
    D3DXVec3Cross(&RIGHT, &LOOK, &UP);
    D3DXVec3Normalize(&RIGHT, &RIGHT);
    D3DXVec3Cross(&UP, &RIGHT, &LOOK);
    D3DXVec3Normalize(&UP, &UP);

    D3DXVECTOR3 LOOK_POS = LOOK + POS;
    D3DXMatrixLookAtRH(&matView, &POS, &LOOK_POS, &UP);
}

MathLib::AABB ShiftEngine::CameraSceneNode::GetBBox() const
{
    return MathLib::AABB();
}
