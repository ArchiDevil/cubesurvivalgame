#include "OOBB.h"

MathLib::OOBB::OOBB(Vector3F _position, Vector3F _size, Vector3F _angles)
	: position(_position), size(_size)
{
//	rotation = MatrixPitchYawRoll();
}

void MathLib::OOBB::GetVertices( Vector3F vertices[8] ) const
{
	vertices[0] = Vector3F(position.x - size.x / 2, position.y - size.x / 2, position.z - size.x / 2);
	vertices[1] = Vector3F(position.x + size.x / 2, position.y - size.x / 2, position.z - size.x / 2);
	vertices[2] = Vector3F(position.x - size.x / 2, position.y + size.x / 2, position.z - size.x / 2);
	vertices[3] = Vector3F(position.x - size.x / 2, position.y - size.x / 2, position.z + size.x / 2);

	vertices[4] = Vector3F(position.x + size.x / 2, position.y + size.x / 2, position.z - size.x / 2);
	vertices[5] = Vector3F(position.x + size.x / 2, position.y - size.x / 2, position.z + size.x / 2);
	vertices[6] = Vector3F(position.x - size.x / 2, position.y + size.x / 2, position.z + size.x / 2);
	vertices[7] = Vector3F(position.x + size.x / 2, position.y + size.x / 2, position.z + size.x / 2);

	for (int i = 0; i < 8 ; i++)
		vertices[i] = rotation * vertices[i];
}

MathLib::AABB MathLib::OOBB::GetAABB() const
{
	Vector3F min, max;
	Vector3F points[8];

	GetVertices(points);

	min.x = points[0].x; min.y = points[0].y; min.z = points[0].z;
	max.x = points[0].x; max.y = points[0].y; max.z = points[0].z;

	for (int i = 0; i < 8 ; i++)
	{
		for (int j = 0; j < 3 ; j++)
		{
			if(points[i].el[j] < min.el[j])
				min.el[j] = points[i].el[j];
		}

		for (int j = 0; j < 3 ; j++)
		{
			if(points[i].el[j] < max.el[j])
				max.el[j] = points[i].el[j];
		}
	}

	return AABB(min, max);
}

bool MathLib::OOBB::IntersectsAABB( const AABB & bbox )
{
	throw;
}

bool MathLib::OOBB::IntersectsOOBB( const OOBB & bbox )
{
	Vector3F a = this->size,
		b = bbox.size,
		Pa = this->position,
		Pb = bbox.position;

	mat3f A = this->rotation,
		B = bbox.rotation;

	//смещение в мировой системе координат
	Vector3F v = Pb - Pa;

	//смещение в системе координат А
	Vector3F T = A * v;

	//создаем матрицу поворота B относительно А
	const mat3f R(A * B.transpose());

	float ra, rb, t;

	//система координат А
	for( int i=0 ; i<3 ; i++ )
	{
		ra = a.el[i]; 
		rb = b.el[0]*fabs(R[i][0]) + b.el[1]*fabs(R[i][1]) + b.el[2]*fabs(R[i][2]);
		t = fabs( T.el[i] );
		if ( t > ra + rb )
			return false;
	}

	//система координат B
	for( int k=0 ; k<3 ; k++ )
	{
		ra = a.el[0]*fabs(R[0][k]) + a.el[1]*fabs(R[1][k]) + a.el[2]*fabs(R[2][k]);
		rb = b.el[k];
		t = fabs( T.el[0]*R[0][k] + T.el[1]*R[1][k] + T.el[2]*R[2][k] );
		if ( t > ra + rb )
			return false;
	}

	//9 векторных произведений
	//L = A0 x B0
	ra = a.el[1]*fabs(R[2][0]) + a.el[2]*fabs(R[1][0]);
	rb = b.el[1]*fabs(R[0][2]) + b.el[2]*fabs(R[0][1]);
	t = fabs( T.el[2]*R[1][0] - T.el[1]*R[2][0] );
	if ( t > ra + rb )
		return false;

	//L = A0 x B1
	ra = a.el[1]*fabs(R[2][1]) + a.el[2]*fabs(R[1][1]);
	rb = b.el[0]*fabs(R[0][2]) + b.el[2]*fabs(R[0][0]);
	t = fabs( T.el[2]*R[1][1] - T.el[1]*R[2][1] );
	if ( t > ra + rb )
		return false;

	//L = A0 x B2
	ra = a.el[1]*fabs(R[2][2]) + a.el[2]*fabs(R[1][2]);
	rb = b.el[0]*fabs(R[0][1]) + b.el[1]*fabs(R[0][0]);
	t = fabs( T.el[2]*R[1][2] - T.el[1]*R[2][2] );
	if ( t > ra + rb )
		return false;

	//L = A1 x B0
	ra = a.el[0]*fabs(R[2][0]) + a.el[2]*fabs(R[0][0]);
	rb = b.el[1]*fabs(R[1][2]) + b.el[2]*fabs(R[1][1]);
	t = fabs( T.el[0]*R[2][0] - T.el[2]*R[0][0] );
	if ( t > ra + rb )
		return false;

	//L = A1 x B1
	ra = a.el[0]*fabs(R[2][1]) + a.el[2]*fabs(R[0][1]);
	rb = b.el[0]*fabs(R[1][2]) + b.el[2]*fabs(R[1][0]);
	t = fabs( T.el[0]*R[2][1] - T.el[2]*R[0][1] );
	if ( t > ra + rb )
		return false;

	//L = A1 x B2
	ra = a.el[0]*fabs(R[2][2]) + a.el[2]*fabs(R[0][2]);
	rb = b.el[0]*fabs(R[1][1]) + b.el[1]*fabs(R[1][0]);
	t = fabs( T.el[0]*R[2][2] - T.el[2]*R[0][2] );
	if ( t > ra + rb )
		return false;

	//L = A2 x B0
	ra = a.el[0]*fabs(R[1][0]) + a.el[1]*fabs(R[0][0]);
	rb = b.el[1]*fabs(R[2][2]) + b.el[2]*fabs(R[2][1]);
	t = fabs( T.el[1]*R[0][0] - T.el[0]*R[1][0] );
	if ( t > ra + rb )
		return false;

	//L = A2 x B1
	ra = a.el[0]*fabs(R[1][1]) + a.el[1]*fabs(R[0][1]);
	rb = b.el[0] *fabs(R[2][2]) + b.el[2]*fabs(R[2][0]);
	t = fabs( T.el[1]*R[0][1] - T.el[0]*R[1][1] );
	if ( t > ra + rb )
		return false;

	//L = A2 x B2
	ra = a.el[0]*fabs(R[1][2]) + a.el[1]*fabs(R[0][2]);
	rb = b.el[0]*fabs(R[2][1]) + b.el[1]*fabs(R[2][0]);
	t = fabs( T.el[1]*R[0][2] - T.el[0]*R[1][2] );
	if ( t > ra + rb )
		return false;

	return true;
}

bool MathLib::OOBB::IntersectsSphere( const Vector3F & center, float raduis )
{
	throw;
}

bool MathLib::OOBB::IntersectsLineSegment( const Vector3F &mid, const Vector3F &dir, const int hl )
{
	const Vector3F sizes = this->size;
	const Vector3F position = this->position;

	Vector3F newMid = this->rotation * mid;
	Vector3F newDir = this->rotation * dir;

	const Vector3F T = position - newMid;

	float r;

	// проверяем, является ли одна из осей X,Y,Z разделяющей
	if ( (fabs(T.x) > sizes.x + hl*fabs(newDir.x)) || 
		(fabs(T.y) > sizes.y + hl*fabs(newDir.y)) ||
		(fabs(T.z) > sizes.z + hl*fabs(newDir.z)) )
		return false;

	// проверяем X ^ dir
	r = sizes.y*fabs(newDir.z) + sizes.z*fabs(newDir.y);
	if ( fabs(T.y*newDir.z - T.z*newDir.y) > r )
		return false;

	// проверяем Y ^ dir
	r = sizes.x*fabs(newDir.z) + sizes.z*fabs(newDir.x);
	if ( fabs(T.z*newDir.x - T.x*newDir.z) > r )
		return false;

	// проверяем Z ^ dir
	r = sizes.x*fabs(newDir.y) + sizes.y*fabs(newDir.x);
	if ( fabs(T.x*newDir.y - T.y*newDir.x) > r )
		return false;

	return true;
}

bool MathLib::OOBB::IntersectsTriangle( const Vector3F &pnt1, const Vector3F &pnt2, const Vector3F &pnt3 )
{
	throw;
}
