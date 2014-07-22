#include "cSelectedBlock.h"

cSelectedBlock::cSelectedBlock() 
	: PosFree(0.0f, 0.0f, 0.0f), Color(1.0f, 0.0f, 0.0f), 
	selectedCubeNode(nullptr)
{
}

cSelectedBlock::~cSelectedBlock()
{
}

void cSelectedBlock::Initialize()
{
	auto cubeMaterial = ShiftEngine::GetContextManager()->LoadMaterial(L"selectedCube.mtl", L"cube");

	selectedCubeNode = ShiftEngine::GetSceneGraph()->AddMeshNode(ShiftEngine::Utilities::createCube(), 
		MathLib::AABB(Vector3F(-0.5f, -0.5f, -0.5f), Vector3F(0.5f, 0.5f, 0.5f)), cubeMaterial.get());
}

void cSelectedBlock::SetPositionFree( const Vector3D & vec )
{
	PosFree = vec;
	selectedCubeNode->SetPosition(PosFree);
	if(vec.z < 0.0f)
		selectedCubeNode->SetVisibility(false);
	else
		selectedCubeNode->SetVisibility(true);
}

void cSelectedBlock::SetPositionSolid( const Vector3D & vec )
{
	PosSolid = vec;
}

bool cSelectedBlock::IsVisible() const
{
	if(PosFree.z > -1)
		return true;
	return false;
}

Pos cSelectedBlock::GetPositions() const
{
	return Pos(PosFree, PosSolid);
}

void cSelectedBlock::SetColor( const Vector3D & color )
{
	Color = color;
	selectedCubeNode->GetMaterialPtr()->SetNamedParam("Color", Color);
}
