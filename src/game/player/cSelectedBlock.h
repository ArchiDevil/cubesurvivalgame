#pragma once

#include "../../MathLib/math.h"
#include "../../GraphicsEngine/ShiftEngine.h"

using MathLib::Vector3D;

struct SelBlockVertex
{
	MathLib::Vector3F Position;
};

struct Pos
{
	Pos(const Vector3D & f, const Vector3D & s) : free(f), solid(s) {};
	const Vector3D free;
	const Vector3D solid;
};

class cSelectedBlock
{
public:
	cSelectedBlock();
	~cSelectedBlock();

	void Initialize();
	void SetPositionFree(const Vector3D & vec);
	void SetPositionSolid(const Vector3D & vec);
	Pos GetPositions() const;
	bool IsVisible() const;
	void SetColor(const Vector3D & color);

private:
	Vector3D PosFree;
	Vector3D PosSolid;
	Vector3D Color;

	ShiftEngine::MeshNode * selectedCubeNode;

};