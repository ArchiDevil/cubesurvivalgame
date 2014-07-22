#include "CameraFrustum.h"

#define OUT 0
#define INTERSECT 1
#define IN 2

ShiftEngine::CameraFrustum::CameraFrustum()
{
}

ShiftEngine::CameraFrustum::~CameraFrustum()
{
}

void ShiftEngine::CameraFrustum::BuildFrustum( D3DXMATRIX * matView, D3DXMATRIX * matProj )
{
	D3DXMATRIX Matrix;

	::ZeroMemory(&planes, sizeof(D3DXPLANE) * 6);
	::ZeroMemory(&Matrix, sizeof(D3DXMATRIX));

	D3DXMatrixMultiply(&Matrix, matView, matProj);

	// Вычисление плоскостей
	planes[0].a = Matrix._14 + Matrix._13; // Задняя плоскость
	planes[0].b = Matrix._24 + Matrix._23;
	planes[0].c = Matrix._34 + Matrix._33;
	planes[0].d = Matrix._44 + Matrix._43;
	D3DXPlaneNormalize(&planes[0], &planes[0]);

	planes[1].a = Matrix._14 - Matrix._13; // Передняя плоскость
	planes[1].b = Matrix._24 - Matrix._23;
	planes[1].c = Matrix._34 - Matrix._33;
	planes[1].d = Matrix._44 - Matrix._43;
	D3DXPlaneNormalize(&planes[1], &planes[1]);

	planes[2].a = Matrix._14 + Matrix._11; // Левая плоскость
	planes[2].b = Matrix._24 + Matrix._21;
	planes[2].c = Matrix._34 + Matrix._31;
	planes[2].d = Matrix._44 + Matrix._41;
	D3DXPlaneNormalize(&planes[2], &planes[2]);

	planes[3].a = Matrix._14 - Matrix._11; // Правая плоскость
	planes[3].b = Matrix._24 - Matrix._21;
	planes[3].c = Matrix._34 - Matrix._31;
	planes[3].d = Matrix._44 - Matrix._41;
	D3DXPlaneNormalize(&planes[3], &planes[3]);

	planes[4].a = Matrix._14 - Matrix._12; // Верхняя плоскость
	planes[4].b = Matrix._24 - Matrix._22;
	planes[4].c = Matrix._34 - Matrix._32;
	planes[4].d = Matrix._44 - Matrix._42;
	D3DXPlaneNormalize(&planes[4], &planes[4]);

	planes[5].a = Matrix._14 + Matrix._12; // Нижняя плоскость
	planes[5].b = Matrix._24 + Matrix._22;
	planes[5].c = Matrix._34 + Matrix._32;
	planes[5].d = Matrix._44 + Matrix._42;
	D3DXPlaneNormalize(&planes[5], &planes[5]);
}

int ShiftEngine::CameraFrustum::CheckAABB( const MathLib::AABB & bbox ) const
{
	MathLib::Vector3F vCorner[8];
	int iTotalIn = 0;

	// get the corners of the box into the vCorner array
	bbox.GetVertices(vCorner);
	D3DXVECTOR3 buffer[8];
	//HACK: very-very ugly hack!
	memcpy(buffer, vCorner, sizeof(float) * 3 * 8);

	// test all 8 corners against the 6 sides 
	// if all points are behind 1 specific plane, we are out
	// if we are in with all points, then we are fully in
	for(int p = 0; p < 6; ++p) 
	{

		int iInCount = 8;
		int iPtIn = 1;

		for(int i = 0; i < 8; ++i) 
		{
			// test this point against the planes
			if(D3DXPlaneDotCoord(&planes[p], &buffer[i]) < 0)	//if point behind plane
			{
				iPtIn = 0;
				--iInCount;
			}
		}

		// were all the points outside of plane p?
		if(iInCount == 0)
			return OUT;

		// check if they were all on the right side of the plane
		iTotalIn += iPtIn;
	}

	// so if iTotalIn is 6, then all are inside the view
	if(iTotalIn == 6)
		return IN;

	// we must be partly in then otherwise
	return INTERSECT;
}

int ShiftEngine::CameraFrustum::CheckOOBB( const MathLib::OOBB & bbox ) const
{
	MathLib::Vector3F vCorner[8];
	int iTotalIn = 0;

	bbox.GetVertices(vCorner);

	for(int p = 0; p < 6; ++p) 
	{
		int iInCount = 8;
		int iPtIn = 1;

		for(int i = 0; i < 8; ++i) 
		{
			D3DXVECTOR3 buffer = D3DXVECTOR3(vCorner[i].x, vCorner[i].y, vCorner[i].z);
			if(D3DXPlaneDotCoord(&planes[p], &buffer) < 0)
			{
				iPtIn = 0;
				--iInCount;
			}
		}

		if(iInCount == 0)
			return OUT;

		iTotalIn += iPtIn;
	}

	if(iTotalIn == 6)
		return IN;

	return INTERSECT;
}

int ShiftEngine::CameraFrustum::CheckQTreeNode( const MathLib::AABB & bbox ) const
{
	int iTotalIn = 0;
	D3DXVECTOR3 buffer[4];
	buffer[0] = D3DXVECTOR3(bbox.bMin.x, bbox.bMin.y, 0.0f);
	buffer[1] = D3DXVECTOR3(bbox.bMin.x, bbox.bMax.y, 0.0f);
	buffer[2] = D3DXVECTOR3(bbox.bMax.x, bbox.bMin.y, 0.0f);
	buffer[3] = D3DXVECTOR3(bbox.bMax.x, bbox.bMax.y, 0.0f);

	for(int p = 0; p < 4; ++p) //last 2 is a top and bottom planes of frustum
	{
		int iInCount = 4;
		int iPtIn = 1;

		for(int i = 0; i < 4; ++i) 
		{
			if(D3DXPlaneDotCoord(&planes[p], &buffer[i]) < 0)
			{
				iPtIn = 0;
				--iInCount;
			}
		}

		if(iInCount == 0)
			return OUT;

		iTotalIn += iPtIn;
	}

	if(iTotalIn == 4)
		return IN;

	return INTERSECT;
}
