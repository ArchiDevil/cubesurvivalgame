#include "../ShiftEngine.h"

#include "SkySceneNode.h"
#include "MeshNode.h"

ShiftEngine::SkySceneNode::SkySceneNode( const Material * material, const MeshDataPtr & meshData )
	: MeshNode(meshData, material, MathLib::AABB())
{
}

ShiftEngine::SkySceneNode::~SkySceneNode()
{
}

void ShiftEngine::SkySceneNode::PushToRQ( RenderQueue & rq )
{
	//auto pos = GetSceneGraph()->GetActiveCamera()->GetPosition();
	//this->SetPosition(Vector3F(pos.x, pos.y, pos.z));
	rq.SetSkyNode(this);
}

int ShiftEngine::SkySceneNode::CheckVisibility( CameraSceneNode * /*activeCam*/ ) const
{
	return 2;
}
