#pragma once

#include "ISceneNode.h"
#include "PlainTreeNode.h"
#include "QuadTreeNode.h"
#include "CameraSceneNode.h"
#include "MeshNode.h"
#include "SkySceneNode.h"

namespace ShiftEngine
{
    enum SceneGraphType
    {
        SGT_Plain,
        SGT_QuadTree,
        //SGT_OctTree
    };

    class ISceneNode;
    class PlainTreeNode;
    class QuadTreeNode;
    class MeshNode;
    class CameraSceneNode;
    class SkySceneNode;
    class LightNode;

    class SceneGraph
    {
    public:
        SceneGraph(SceneGraphType graphType = SGT_Plain);
        ~SceneGraph();

        void DrawAll(double dt) const;

        MeshNode * AddMeshNode(const std::wstring & meshFileName, const Material * mat);                        //tries to load mesh with meshLoader
        MeshNode * AddMeshNode(IMeshDataPtr dataPtr, const MathLib::AABB & bbox, const Material * mat);
        CameraSceneNode * AddCameraSceneNode();

        LightNode * AddDirectionalLightNode(const Vector3F & direction, const Vector3F & color = Vector3F(1.0f, 1.0f, 1.0f));
        void RemoveDirectionalLightNode(LightNode * node);

        LightNode * AddPointLightNode(const Vector3F & pos, float radius, const Vector3F & color = Vector3F(1.0f, 1.0f, 1.0f));
        SkySceneNode * AddSkySceneNode();

        SkySceneNode * GetActiveSkyNode() const;

        void SetActiveCamera(CameraSceneNode * camera);
        CameraSceneNode * GetActiveCamera() const;

        void SetAmbientColor(const MathLib::Vector3F & color);
        MathLib::Vector3F GetAmbientColor() const;

        void MoveNodeCallback(ISceneNode * node);

    protected:
        ISceneNode * rootNode;
        CameraSceneNode * activeCamera;
        SkySceneNode * activeSky;
        VertexSemantic skySemantic;
        std::vector<LightNode*> directionalLights;

        SceneGraphType type;

        MathLib::Vector3F ambientColor;

    };
}
