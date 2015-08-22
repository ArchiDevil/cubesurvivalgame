#include "../ShiftEngine.h"
#include "SceneGraph.h"
#include "../RenderQueue.h"

#include <cassert>

ShiftEngine::SceneGraph::SceneGraph(SceneGraphType graphType /*= SGT_Plain*/)
    : activeCamera(nullptr)
    , activeSky(nullptr)
    , ambientColor(0.0f, 0.0f, 0.0f)
    , type(graphType)
{
    skySemantic.addSemantic(ET_FLOAT, 3, ES_Position);
    GetContextManager()->GetVertexDeclaration(skySemantic);

    switch (graphType)
    {
    case ShiftEngine::SGT_Plain:
        rootNode = new PlainTreeNode();
        break;
    case ShiftEngine::SGT_QuadTree:
        rootNode = new QuadTreeNode(-2048.0f, 2048.0f, -2048.0f, 2048.0f);
        break;
    default:
        assert(false);
        break;
    }
    rootNode->SetSceneGraph(this);
}

ShiftEngine::SceneGraph::~SceneGraph()
{
    if (rootNode)
        rootNode->release();
}

void ShiftEngine::SceneGraph::DrawAll(double dt) const
{
    RenderQueue rq(ambientColor);

    if (activeCamera)
        activeCamera->Update();
    if (activeCamera)
        rq.SetCameraNode(activeCamera);
    if (activeSky)
        rq.SetSkyNode(activeSky);

    for (auto iter : directionalLights)
        iter->Draw(rq);

    rootNode->Draw(rq);

    GetRenderer()->DrawAll(rq, dt);
}

ShiftEngine::MeshNode * ShiftEngine::SceneGraph::AddMeshNode(const std::wstring & meshFileName, const Material * material)
{
    auto pCtxMgr = GetContextManager();
    IMeshDataPtr data = pCtxMgr->LoadMesh(meshFileName);
    MeshNode * out = new MeshNode(data, material, MathLib::AABB({ -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }));
    out->SetSceneGraph(this);
    rootNode->AddChild(out);
    return out;
}

ShiftEngine::MeshNode * ShiftEngine::SceneGraph::AddMeshNode(IMeshDataPtr dataPtr, const MathLib::AABB & bbox, const Material * mat)
{
    MeshNode * out = new MeshNode(dataPtr, mat, bbox);
    rootNode->AddChild(out);
    out->SetSceneGraph(this);
    return out;
}

ShiftEngine::CameraSceneNode * ShiftEngine::SceneGraph::AddCameraSceneNode()
{
    auto ContextManager = GetContextManager();

    CameraSceneNode * cam = new CameraSceneNode();
    cam->SetSceneGraph(this);
    rootNode->AddChild(cam);
    cam->Initialize((float)ContextManager->GetEngineSettings().screenWidth,
                    (float)ContextManager->GetEngineSettings().screenHeight,
                    ContextManager->GetEngineSettings().zNear,
                    ContextManager->GetEngineSettings().zFar,
                    60.0f);

    if (!activeCamera)
        activeCamera = cam;

    return cam;
}

void ShiftEngine::SceneGraph::SetActiveCamera(CameraSceneNode * camera)
{
    this->activeCamera = camera;
}

ShiftEngine::CameraSceneNode * ShiftEngine::SceneGraph::GetActiveCamera() const
{
    return activeCamera;
}

ShiftEngine::SkySceneNode * ShiftEngine::SceneGraph::AddSkySceneNode()
{
    ShiftEngine::Material material(GetContextManager()->LoadShader(L"sky.fx"));
    ShiftEngine::IMeshDataPtr mesh = GetContextManager()->LoadMesh(L"sky.lim");
    SkySceneNode * out = new SkySceneNode(&material, mesh);
    activeSky = out;
    out->SetSceneGraph(this);
    return out;
}

ShiftEngine::LightNode * ShiftEngine::SceneGraph::AddDirectionalLightNode(const MathLib::Vector3F & direction, const MathLib::Vector3F & color)
{
    LightNode * out = new LightNode(LNT_Directional, color);
    out->SetDirection(direction);
    out->SetSceneGraph(this);
    out->addRef();
    directionalLights.push_back(out);
    return out;
}

ShiftEngine::LightNode * ShiftEngine::SceneGraph::AddPointLightNode(const MathLib::Vector3F & pos, float radius, const MathLib::Vector3F & color)
{
    LightNode * out = new LightNode(LNT_Point, color);
    out->SetRadius(radius);
    out->SetPosition(pos);
    out->SetSceneGraph(this);
    rootNode->AddChild(out);
    return out;
}

void ShiftEngine::SceneGraph::SetAmbientColor(const MathLib::Vector3F & color)
{
    ambientColor = color;
}

MathLib::Vector3F ShiftEngine::SceneGraph::GetAmbientColor() const
{
    return ambientColor;
}

ShiftEngine::SkySceneNode * ShiftEngine::SceneGraph::GetActiveSkyNode() const
{
    return activeSky;
}

void ShiftEngine::SceneGraph::RemoveDirectionalLightNode(LightNode * node)
{
    for (auto iter = directionalLights.begin(); iter != directionalLights.end(); ++iter)
    {
        if (*iter != node)
            continue;

        (*iter)->release();
        iter = directionalLights.erase(iter);
        if (iter == directionalLights.end())
            return;
    }
}

void ShiftEngine::SceneGraph::MoveNodeCallback(ISceneNode * node)
{
    switch (type)
    {
    case ShiftEngine::SGT_Plain:
        return;
    case ShiftEngine::SGT_QuadTree:
    {
        node->addRef();
        node->RemoveParent();
        rootNode->AddChild(node);
        node->release();
    }
    break;
    default:
        assert(false);
    }
}
