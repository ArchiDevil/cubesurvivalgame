#pragma once

#include "ISceneNode.h"
#include "CameraFrustum.h"

#include <MathLib/math/quaternion.h>

//REMOVE D3DX LIB !!!

namespace ShiftEngine
{
	static const float MAX_ANGLE = 89.5f;

	class CameraSceneNode : public ISceneNode
	{
	public:
		CameraSceneNode(
			D3DXVECTOR3 _pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
			D3DXVECTOR3 _up = D3DXVECTOR3(0.0f, 0.0f, 1.0f), 
			D3DXVECTOR3 _right = D3DXVECTOR3(1.0f, 0.0f, 0.0f));
		~CameraSceneNode();

		void Initialize(float _screenWidth, float _screenHeight, float _zNear, float _zFar, float _FOV);

		void SetPosition(float x, float y, float z);
		void SetPosition(const D3DXVECTOR3 & pos);
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

		virtual void PushToRQ( RenderQueue & rq ) override;

		const D3DXMATRIX & GetProjectionMatrix() const;
		const D3DXMATRIX & GetViewMatrix() const;

		float GetZNear() const;
		float GetZFar() const;
		float GetFOV() const;

		void SetZNear(float val);
		void SetZFar(float val);
		void SetFOV(float val);
		void SetScreenWidth(float val);
		void SetScreenHeight(float val);

	private:
		void RebuildProjMatrix();

		D3DXMATRIX matView;
		D3DXMATRIX matProj;

		float zNear;
		float zFar;
		float FOV;
		float screenWidth;
		float screenHeight;
		CameraFrustum * Frustum;

		float ViewAngle;
		D3DXVECTOR3 Angles;

		D3DXVECTOR3 UP;
		D3DXVECTOR3 LOOK;
		D3DXVECTOR3 POS;
		D3DXVECTOR3 RIGHT;

	};
}
