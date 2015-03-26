#pragma once

#include <MathLib/math.h>

#include <vector>

//This class provides storage for scene nodes that will be rendered by renderer.
namespace ShiftEngine
{
    class ISceneNode;
    class MeshNode;
    class CameraSceneNode;
    class SkySceneNode;
    class LightNode;

    typedef std::vector<MeshNode*> RenderVector;
    typedef std::vector<LightNode*> LightsVector;

    class RenderQueue
    {
    public:
        RenderQueue(const MathLib::Vector3F & ambientColor);
        ~RenderQueue();

        void AddRenderableNode(MeshNode * node);
        void SetCameraNode(CameraSceneNode * node);
        void SetSkyNode(SkySceneNode * node);
        void AddLightNode(LightNode * node);

        RenderVector & GetRenderableNodes();
        const LightsVector & GetLights() const;

        CameraSceneNode * GetActiveCamera() const;
        SkySceneNode * GetActiveSky() const;

        MathLib::Vector3F GetAmbientColor() const;

    private:
        RenderVector meshNodesVector;
        LightsVector lights;
        MathLib::Vector3F ambientColor = { 0.0f, 0.0f, 0.0f };
        CameraSceneNode * activeCamera = nullptr;
        SkySceneNode * activeSky = nullptr;

    };
}
