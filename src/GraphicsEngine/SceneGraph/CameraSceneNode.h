#pragma once

#include "ISceneNode.h"
#include "CameraFrustum.h"

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

		void SetPosition(float x, float y, float z);	//ставит позицию для камеры
		void SetPosition(const D3DXVECTOR3 & pos);		//ставит позицию для камеры

		void Update();									//делает пересборку матриц

		void MoveUpDown(float units);					//двигает камеру по вертикальной оси
		void MoveLeftRight(float units);				//двигает камеру по правой оси 
		void MoveForwardBackward(float units);			//двигает камеру по передней оси

		void LookLeftRight(float angle);				//вращает камеру по вертикальному вектору
		void LookUpDown(float angle);					//вращает камеру по правому вектору
		void LookAt(D3DXVECTOR3 point);					//undocumented

		D3DXVECTOR3 GetLookVector() const;				//возвращает вектор вгляда
		D3DXVECTOR3 GetRightVector() const;				//возвращает правый вектор
		D3DXVECTOR3 GetPosition() const;				//возвращает позицию камеры
		D3DXVECTOR3 GetAngles() const;					//возвращает углы поворота

		CameraFrustum * GetFrustumPtr();				//возвращает указатель на фрустум

		virtual void PushToRQ( RenderQueue & rq );

		D3DXMATRIX & GetProjectionMatrix();
		D3DXMATRIX & GetViewMatrix();

	private:
		D3DXMATRIX matView;
		D3DXMATRIX matProj;

		float zNear;
		float zFar;
		float FOV;

		CameraFrustum * Frustum;

		float ViewAngle;								//угол вверх\вниз, нужно для ограничения обзора, чтобы нельзя было перевернуться через голову
		D3DXVECTOR3 Angles;

		D3DXVECTOR3 UP;
		D3DXVECTOR3 LOOK;
		D3DXVECTOR3 POS;
		D3DXVECTOR3 RIGHT;
		D3DXVECTOR3 POS_ZERO;

	};
}