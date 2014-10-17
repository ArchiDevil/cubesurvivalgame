#pragma once

#include "matrix.h"

namespace MathLib
{
	/*
	D3DXColorAdjustContrast 
	D3DXColorAdjustSaturation 
	D3DXCreateMatrixStack 
	D3DXCpuOptimizations 
	D3DXFloat16To32Array 
	D3DXFloat32To16Array 
	D3DXFresnelTerm 

	D3DXPlaneFromPointNormal 
	D3DXPlaneFromPoints 
	D3DXPlaneIntersectLine 
	D3DXPlaneNormalize 
	D3DXPlaneTransform 
	D3DXPlaneTransformArray 

	D3DXQuaternionBaryCentric 
	D3DXQuaternionExp 
	D3DXQuaternionInverse 
	D3DXQuaternionLn 
	D3DXQuaternionMultiply 
	D3DXQuaternionNormalize 
	D3DXQuaternionRotationAxis 
	D3DXQuaternionRotationMatrix 
	D3DXQuaternionRotationYawPitchRoll 
	D3DXQuaternionSlerp 
	D3DXQuaternionSquad 
	D3DXQuaternionSquadSetup 
	D3DXQuaternionToAxisAngle 

	D3DXSHEvalConeLight 
	D3DXSHEvalDirection 
	D3DXSHEvalDirectionalLight 
	D3DXSHEvalHemisphereLight 
	D3DXSHMultiply2 
	D3DXSHMultiply3 
	D3DXSHMultiply4 
	D3DXSHMultiply5 
	D3DXSHMultiply6 
	D3DX10SHProjectCubeMap 
	D3DXSHRotate 
	D3DXSHRotateZ 
	D3DXSHScale 

	D3DXVec2BaryCentric 
	D3DXVec2CatmullRom 
	D3DXVec2Hermite 
	D3DXVec2Transform 
	D3DXVec2TransformArray 
	D3DXVec2TransformCoord 
	D3DXVec2TransformCoordArray 
	D3DXVec2TransformNormal 
	D3DXVec2TransformNormalArray 

	D3DXVec3BaryCentric 
	D3DXVec3CatmullRom 
	D3DXVec3Hermite 
	D3DXVec3Project 
	D3DXVec3ProjectArray 
	D3DXVec3Transform 
	D3DXVec3TransformArray 
	D3DXVec3TransformCoord 
	D3DXVec3TransformCoordArray 
	D3DXVec3TransformNormal 
	D3DXVec3TransformNormalArray 

	D3DXVec4BaryCentric 
	D3DXVec4CatmullRom 
	D3DXVec4Cross 
	D3DXVec4Hermite 
	D3DXVec4Transform 
	D3DXVec4TransformArray
	*/

	//D3DXMatrixAffineTransformation 
	//D3DXMatrixAffineTransformation2D 
	//D3DXMatrixDecompose

	template <typename T>
	matrix<T, 4> matrixIdentity()
	{
		matrix<T, 4> out;
		for(int i = 0; i < 4; i++)
		{
			out[i][i] = (T)1;
		}
		return out;
	}

	template <typename T>
	matrix<T, 4> matrixScale(T x, T y, T z)
	{
		matrix<T, 4> out;
		out[0][0] = x;
		out[1][1] = y;
		out[2][2] = z;
		out[3][3] = (T)1;
		return out;
	}

	template<typename T>
	matrix<T, 4> matrixRotationX(T value)
	{
		matrix<T, 4> matX;
		matX[0][0] = (T)1;
		matX[1][1] = cos(value);
		matX[1][2] = sin(value);
		matX[2][1] = -sin(value);
		matX[2][2] = cos(value);
		matX[3][3] = (T)1;
		return matX;
	}

	template<typename T>
	matrix<T, 4> matrixRotationY(T value)
	{
		matrix<T, 4> matY;
		matY[0][0] = cos(value);
		matY[0][2] = -sin(value);
		matY[1][1] = (T)1;
		matY[2][0] = sin(value);
		matY[2][2] = cos(value);
		matY[3][3] = (T)1;
		return matY;
	}

	template<typename T>
	matrix<T, 4> matrixRotationZ(T value)
	{
		matrix<T, 4> matZ;
		matZ[0][0] = cos(value);
		matZ[0][1] = sin(value);
		matZ[1][0] = -sin(value);
		matZ[1][1] = cos(value);
		matZ[2][2] = (T)1;
		matZ[3][3] = (T)1;
		return matZ;
	}

	template<typename T>
	matrix<T, 4> matrixRotationYPR(T yaw, T pitch, T roll)
	{
		matrix<T, 4> out;
		out[0][0] = ( cos(roll) * cos(yaw) ) + ( sin(roll) * sin(pitch) * sin(yaw) );
		out[0][1] = ( sin(roll) * cos(pitch) );
		out[0][2] = ( cos(roll) * -sin(yaw) ) + ( sin(roll) * sin(pitch) * cos(yaw) );
		out[1][0] = (-sin(roll) * cos(yaw) ) + ( cos(roll) * sin(pitch) * sin(yaw) );
		out[1][1] = ( cos(roll) * cos(pitch) );
		out[1][2] = ( sin(roll) * sin(yaw) ) + ( cos(roll) * sin(pitch) * cos(yaw) );
		out[2][0] = ( cos(pitch) * sin(yaw) );
		out[2][1] = -sin(pitch);
		out[2][2] = ( cos(pitch) * cos(yaw) );
		out[3][3] = (T)1;
		return out;
	}

	template <typename T>
	matrix<T, 4> matrixTranslation(T x, T y, T z)
	{
		matrix<T, 4> out;
		out[0][0] = (T)1;
		out[1][1] = (T)1;
		out[2][2] = (T)1;
		out[3][3] = (T)1;

		out[3][0] = x;
		out[3][1] = y;
		out[3][2] = z;
		return out;
	}

	//D3DXMatrixLookAtLH 
	//D3DXMatrixLookAtRH 
	//D3DXMatrixOrthoLH 
	//D3DXMatrixOrthoRH 
	//D3DXMatrixOrthoOffCenterLH 
	//D3DXMatrixOrthoOffCenterRH 
	//D3DXMatrixPerspectiveFovLH 
	//D3DXMatrixPerspectiveFovRH 
	//D3DXMatrixReflect 
	//D3DXMatrixRotationAxis 
	//D3DXMatrixShadow 
}
