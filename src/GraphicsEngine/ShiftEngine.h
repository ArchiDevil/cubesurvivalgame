#pragma once

#include "IContextManager.h"
#include "Renderer.h"
#include "SceneGraph/SceneGraph.h"
#include "Utils.h"

namespace ShiftEngine
{
    //TODO: remove window handle and replace it by something cross-platform
    bool InitEngine(const GraphicEngineSettings & settings, const PathSettings & paths, HWND hwnd);
    void ShutdownEngine();

    IContextManager * GetContextManager();
    SceneGraph * GetSceneGraph();
    Renderer * GetRenderer();
}
