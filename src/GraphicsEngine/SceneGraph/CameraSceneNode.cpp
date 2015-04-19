#include "CameraSceneNode.h"

#include "../RenderQueue.h"

ShiftEngine::CameraSceneNode::CameraSceneNode()
    : ISceneNode()
{
}

ShiftEngine::CameraSceneNode::CameraSceneNode(MathLib::Vector3F pos, MathLib::Vector3F up, MathLib::Vector3F right)
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

    Vector3F LOOK_POS = LOOK + POS;
    matView = MathLib::matrixLookAtRH<float, 4>(POS, LOOK_POS, UP);
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
    Vector3F LOOK_POS = LOOK + POS;
    matView = MathLib::matrixLookAtRH<float, 4>(POS, LOOK_POS, UP);
    Frustum->BuildFrustum(matView, matProj);
}

MathLib::Vector3F ShiftEngine::CameraSceneNode::GetLookVector() const
{
    return LOOK;
}

MathLib::Vector3F ShiftEngine::CameraSceneNode::GetRightVector() const
{
    return RIGHT;
}

MathLib::Vector3F ShiftEngine::CameraSceneNode::GetPosition() const
{
    return POS;
}

ShiftEngine::CameraFrustum * ShiftEngine::CameraSceneNode::GetFrustumPtr()
{
    return Frustum.get();
}

void ShiftEngine::CameraSceneNode::LookAt(const MathLib::Vector3F & point)
{
    LOOK = MathLib::normalize(point);
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
    matProj = MathLib::matrixPerspectiveFovRH<float, 4>(M_PIF * FOV / 180.0f,       // vertical FoV
                                                        screenWidth / screenHeight, // screen rate
                                                        zNear,
                                                        zFar);
}

void ShiftEngine::CameraSceneNode::RotateByQuaternion(const MathLib::qaFloat & quat)
{
    // transform all vectors
    Vector3F look(LOOK.x, LOOK.y, LOOK.z);
    look = look * quat;
    LOOK = look;

    Vector3F up(UP.x, UP.y, UP.z);
    up = up * quat;
    UP = up;

    Vector3F right(RIGHT.x, RIGHT.y, RIGHT.z);
    right = right * quat;
    RIGHT = right;

    Vector3F LOOK_POS = LOOK + POS;
    matView = MathLib::matrixLookAtRH<float, 4>(POS, LOOK_POS, UP);
}

void ShiftEngine::CameraSceneNode::SetSphericalCoords(const Vector3F & lookPoint, float phi, float theta, float r)
{
    float x = r * sin(theta * 0.0175f) * cos(phi * 0.0175f);
    float y = r * sin(theta * 0.0175f) * sin(phi * 0.0175f);
    float z = r * cos(theta * 0.0175f);

    POS = Vector3F(x, y, z) + lookPoint;
    LOOK = lookPoint - POS;
    LOOK = normalize(LOOK);
    // we assume that UP vector is always direct to up
    UP = { 0.0f, 0.0f, 1.0f };
    RIGHT = MathLib::cross(LOOK, UP);
    RIGHT = normalize(RIGHT);
    UP = MathLib::cross(RIGHT, LOOK);
    UP = normalize(UP);

    Vector3F LOOK_POS = LOOK + POS;
    matView = MathLib::matrixLookAtRH<float, 4>(POS, LOOK_POS, UP);
}

MathLib::AABB ShiftEngine::CameraSceneNode::GetBBox() const
{
    return MathLib::AABB();
}
