#include "LightNode.h"

#include "../RenderQueue.h"
#include "SceneGraph.h"

ShiftEngine::LightNode::LightNode(LightNodeType _type, const MathLib::Vector3F & _color)
	: ISceneNode(MathLib::AABB()), color(_color), 
	type(_type), active(true)
{
}

ShiftEngine::LightNode::~LightNode()
{
}

void ShiftEngine::LightNode::PushToRQ( RenderQueue & rq )
{
	if(active)
		rq.AddLightNode(this);
}

void ShiftEngine::LightNode::SetColor( const MathLib::Vector3F & color )
{
	this->color = color;
}

MathLib::Vector3F ShiftEngine::LightNode::GetColor() const
{
	return color;
}

ShiftEngine::LightNodeType ShiftEngine::LightNode::GetType() const
{
	return type;
}

float ShiftEngine::LightNode::GetRadius() const
{
	return radius;
}

void ShiftEngine::LightNode::SetRadius( float val )
{
	radius = val;
}

MathLib::Vector3F ShiftEngine::LightNode::GetDirection() const
{
	return direction;
}

void ShiftEngine::LightNode::SetDirection( const MathLib::Vector3F & val )
{
	direction = val;
}

void ShiftEngine::LightNode::SetActive( bool active )
{
	this->active = active;
}

bool ShiftEngine::LightNode::IsActive() const
{
	return active;
}

void ShiftEngine::LightNode::KillSelf()
{
	if(ISceneNode::pSceneGraph)
	{
		if(type == LNT_Directional)
			ISceneNode::pSceneGraph->RemoveDirectionalLightNode(this);
		else
			ISceneNode::KillSelf();
	}
}
