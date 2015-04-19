#pragma once

#include "IContextManager.h"
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

    IContextManager * GetContextManager();
    SceneGraph * GetSceneGraph();
    Renderer * GetRenderer();
}
