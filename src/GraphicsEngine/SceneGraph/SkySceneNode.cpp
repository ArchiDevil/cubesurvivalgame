#include "../ShiftEngine.h"

#include "SkySceneNode.h"
#include "MeshNode.h"

ShiftEngine::SkySceneNode::SkySceneNode(const Material * material, const IMeshDataPtr & meshData)
    : MeshNode(meshData, material)
{
}

ShiftEngine::SkySceneNode::~SkySceneNode()
{
}

void ShiftEngine::SkySceneNode::PushToRQ(RenderQueue & rq)
{
    //auto pos = GetSceneGraph()->GetActiveCamera()->GetPosition();
    //this->SetPosition(Vector3F(pos.x, pos.y, pos.z));
    rq.SetSkyNode(this);
}

int ShiftEngine::SkySceneNode::CheckVisibility(CameraSceneNode * /*activeCam*/) const
{
    return CameraFrustum::CS_In;
}
