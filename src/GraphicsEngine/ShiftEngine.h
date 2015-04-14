#pragma once

#include "APIs/D3D10/D3D10ContextManager.h"
#include "Renderer.h"
#include "SceneGraph/SceneGraph.h"
#include "Utils.h"

namespace ShiftEngine
{
    enum API_TYPE
    {
        AT_DX10,
        AT_DX11,
        AT_OGL33
    };

    bool InitEngine(API_TYPE apiType, GraphicEngineSettings settings, PathSettings paths, HWND hwnd);
    void ShutdownEngine();

    D3D10ContextManager * GetContextManager();
    SceneGraph * GetSceneGraph();
    Renderer * GetRenderer();
}
