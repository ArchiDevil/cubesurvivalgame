#include "CameraSceneNode.h"

#include "../RenderQueue.h"

ShiftEngine::CameraSceneNode::CameraSceneNode(D3DXVECTOR3 _pos, 
											  D3DXVECTOR3 _up, 
											  D3DXVECTOR3 _right)
	: UP(_up), POS(_pos), RIGHT(_right),
	LOOK(0.0f, 1.0f, 0.0f), POS_ZERO (0.0f, 0.0f, 0.0f),
	Angles(0.0f, 0.0f, 0.0f), ISceneNode(MathLib::AABB()),
	zNear(0), zFar(0), FOV(0), Frustum(nullptr), ViewAngle(0)
{
}

ShiftEngine::CameraSceneNode::~CameraSceneNode()
{
}

void ShiftEngine::CameraSceneNode::Initialize(float _screenWidth, float _screenHeight, float _zNear, float _zFar, float _FOV)
{
	zNear = _zNear;
	zFar = _zFar;
	FOV = _FOV;
	
	D3DXVECTOR3 LOOK_POS;
	LOOK_POS = LOOK + POS;
	D3DXMatrixLookAtRH(&matView, &POS, &LOOK_POS, &UP);
	// установка матрицы проекции
	D3DXMatrixPerspectiveFovRH(
		&matProj,
		(float)D3DX_PI * _FOV / 180,	// вертикальный угол обзора
		_screenWidth / _screenHeight,	// отношение сторон экрана
		_zNear,							// координата ближней плоскости обрезки
		_zFar);							// дальняя плоскость обрезки

	Frustum = new CameraFrustum;
}

void ShiftEngine::CameraSceneNode::SetPosition (float x, float y, float z)
{
	POS.x = x;
	POS.y = y;
	POS.z = z;
}

void ShiftEngine::CameraSceneNode::SetPosition(const D3DXVECTOR3 & pos)
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

void ShiftEngine::CameraSceneNode::LookLeftRight(float angle)
{
	D3DXMATRIX TEMP;
	D3DXMatrixLookAtRH(&TEMP, &POS_ZERO, &LOOK, &UP);
	D3DXMatrixRotationAxis(&TEMP, &UP, angle * static_cast<float>(0.0175));
	D3DXVec3TransformCoord(&LOOK, &LOOK, &TEMP);
	D3DXVec3TransformCoord(&RIGHT, &RIGHT, &TEMP);
}

void ShiftEngine::CameraSceneNode::LookUpDown(float angle)
{
	D3DXMATRIX TEMP;
	if (ViewAngle + angle > MAX_ANGLE || ViewAngle + angle < -MAX_ANGLE)
		return;
	ViewAngle += angle;
	D3DXMatrixLookAtRH(&TEMP, &POS_ZERO, &LOOK, &UP);
	D3DXMatrixRotationAxis(&TEMP, &RIGHT, angle * static_cast<float>(0.0175));
	D3DXVec3TransformCoord(&LOOK, &LOOK, &TEMP);
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

ShiftEngine::CameraFrustum * ShiftEngine::CameraSceneNode::GetFrustumPtr()
{
	return Frustum;
}

D3DXVECTOR3 ShiftEngine::CameraSceneNode::GetPosition() const
{
	return POS;
}

void ShiftEngine::CameraSceneNode::LookAt( D3DXVECTOR3 point )
{
	LOOK = point;
	D3DXVec3Normalize(&LOOK, &point);
}

D3DXVECTOR3 ShiftEngine::CameraSceneNode::GetAngles() const
{
	return Angles;
}

void ShiftEngine::CameraSceneNode::PushToRQ( RenderQueue & rq )
{
}

D3DXMATRIX & ShiftEngine::CameraSceneNode::GetProjectionMatrix()
{
	return matProj;
}

D3DXMATRIX & ShiftEngine::CameraSceneNode::GetViewMatrix()
{
	return matView;
}