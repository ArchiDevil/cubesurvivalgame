#include "MeshNode.h"

#include "../ShiftEngine.h"

ShiftEngine::MeshNode::MeshNode(const MeshDataPtr & _data, const Material * mat, const MathLib::AABB & _bbox)
	: ISceneNode(_bbox), isVisible(true),
	material(*mat), Data(_data)
{
}

ShiftEngine::MeshNode::~MeshNode()
{
}

void ShiftEngine::MeshNode::PushToRQ( RenderQueue & rq )
{
	if(this->IsVisible())
		rq.AddRenderableNode(this);
}

bool ShiftEngine::MeshNode::IsVisible() const
{
	return isVisible;
}

void ShiftEngine::MeshNode::SetVisibility( bool vis )
{
	isVisible = vis;
}

ShiftEngine::Material * ShiftEngine::MeshNode::GetMaterialPtr()
{
	return &material;
}

void ShiftEngine::MeshNode::SetMaterial( const ShiftEngine::Material * val )
{
	material = Material(*val);
}

ShiftEngine::MeshDataPtr ShiftEngine::MeshNode::GetDataPtr() const
{
	return Data;
}

void ShiftEngine::MeshNode::SetDataPtr( MeshDataPtr data )
{
	Data = data;
}

int ShiftEngine::MeshNode::Render()
{
	if(!Data)
		return 0;
	return GetContextManager()->DrawMesh(Data);
}
