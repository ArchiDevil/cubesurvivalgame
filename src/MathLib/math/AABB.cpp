#include "AABB.h"

using std::min;
using std::abs;
using std::max;

MathLib::AABB::AABB( const Vector3F & _bmin /*= Vector3F(0.0f, 0.0f, 0.0f)*/,
					const Vector3F & _bmax /*= Vector3F(0.0f, 0.0f, 0.0f)*/ )
					: bMin(_bmin), bMax(_bmax)
{
}

void MathLib::AABB::GetVertices( Vector3F vertices[8] ) const
{
	vertices[0] = Vector3F(bMin);
	vertices[1] = Vector3F(bMax.x, bMin.y, bMin.z);
	vertices[2] = Vector3F(bMin.x, bMax.y, bMin.z);
	vertices[3] = Vector3F(bMax.x, bMax.y, bMin.z);

	vertices[4] = Vector3F(bMin.x, bMin.y, bMax.z);
	vertices[5] = Vector3F(bMin.x, bMax.y, bMax.z);
	vertices[6] = Vector3F(bMax.x, bMin.y, bMax.z);
	vertices[7] = Vector3F(bMax);
}

bool MathLib::AABB::IntersectsAABB( const AABB & otherAABBox ) const
{
	return
		(bMin.x <= otherAABBox.bMax.x) && (bMax.x >= otherAABBox.bMin.x) &&
		(bMin.y <= otherAABBox.bMax.y) && (bMax.y >= otherAABBox.bMin.y) &&
		(bMin.z <= otherAABBox.bMax.z) && (bMax.z >= otherAABBox.bMin.z);
}

void MathLib::AABB::Include( const AABB & otherAABBox )
{
	bMin.x = min(bMin.x, otherAABBox.bMin.x);
	bMin.y = min(bMin.y, otherAABBox.bMin.y);
	bMin.z = min(bMin.z, otherAABBox.bMin.z);

	bMax.x = max(bMax.x, otherAABBox.bMax.x);
	bMax.y = max(bMax.y, otherAABBox.bMax.y);
	bMax.z = max(bMax.z, otherAABBox.bMax.z);
}

float MathLib::AABB::GetSurfaceArea() const
{
	float l = abs(bMax.x - bMin.x);
	float h = abs(bMax.y - bMin.y);
	float w = abs(bMax.z - bMin.z);

	return 2.0f * (l * w + l * h + w * h);
}

float MathLib::AABB::GetVolume() const
{
	float l = abs(bMax.x - bMin.x);
	float h = abs(bMax.y - bMin.y);
	float w = abs(bMax.z - bMin.z);

	return l * h * w;
}

bool MathLib::AABB::IntersectsLineSegment( const Vector3F &mid, const Vector3F &dir, const int hl ) const
{
	const Vector3F sizes = Vector3F((bMax.x - bMin.x) / 2, (bMax.y - bMin.y) / 2, (bMax.z - bMin.z) / 2);
	const Vector3F position = Vector3F(bMax.x - sizes.x, bMax.y - sizes.y, bMax.z - sizes.z);
	const Vector3F T = position - mid;
	float r;

	// проверяем, является ли одна из осей X,Y,Z разделяющей
	if ( (fabs(T.x) > sizes.x + hl*fabs(dir.x)) ||
		(fabs(T.y) > sizes.y + hl*fabs(dir.y)) ||
		(fabs(T.z) > sizes.z + hl*fabs(dir.z)) )
		return false;

	// проверяем X ^ dir
	r = sizes.y*fabs(dir.z) + sizes.z*fabs(dir.y);
	if ( fabs(T.y*dir.z - T.z*dir.y) > r )
		return false;

	// проверяем Y ^ dir
	r = sizes.x*fabs(dir.z) + sizes.z*fabs(dir.x);
	if ( fabs(T.z*dir.x - T.x*dir.z) > r )
		return false;

	// проверяем Z ^ dir
	r = sizes.x*fabs(dir.y) + sizes.y*fabs(dir.x);
	if ( fabs(T.x*dir.y - T.y*dir.x) > r )
		return false;

	return true;
}
