#include "ShiftEngine.h"

namespace ShiftEngine
{
    D3D10ContextManager * ContextManagerInstance;
    SceneGraph * SceneGraphInstance;
    Renderer * RendererInstance;

    bool InitDX10Api(HWND hwnd, GraphicEngineSettings settings, PathSettings paths)
    {
        ContextManagerInstance = new D3D10ContextManager(hwnd);
        if (!ContextManagerInstance->Initialize(settings, paths))
            return false;
        RendererInstance = new Renderer(ContextManagerInstance->GetShaderManager(), ContextManagerInstance->GetShaderGenerator());
        SceneGraphInstance = new SceneGraph(SceneGraphType::SGT_QuadTree);
        return true;
    }

    bool InitDX11Api(HWND /*hwnd*/, GraphicEngineSettings /*settings*/, PathSettings /*paths*/)
    {
        return false;
    }

    bool InitGL33Api(HWND /*hwnd*/, GraphicEngineSettings /*settings*/, PathSettings /*paths*/)
    {
        return false;
    }

    bool InitEngine(API_TYPE apiType, GraphicEngineSettings settings, PathSettings paths, HWND hwnd)
    {
        static bool initialized = false;
        switch (apiType)
        {
        case ShiftEngine::AT_DX10:
            initialized = InitDX10Api(hwnd, settings, paths);
            break;
        case ShiftEngine::AT_DX11:
            initialized = InitDX11Api(hwnd, settings, paths);
            break;
        case ShiftEngine::AT_OGL33:
            initialized = InitGL33Api(hwnd, settings, paths);
            break;
        default:
            break;
        }
        return initialized;
    }

    D3D10ContextManager * GetContextManager()
    {
        return ContextManagerInstance;
    }

    SceneGraph * GetSceneGraph()
    {
        return SceneGraphInstance;
    }

    Renderer * GetRenderer()
    {
        return RendererInstance;
    }

    void ShutdownEngine()
    {
        delete SceneGraphInstance;
        delete RendererInstance;
        ContextManagerInstance->Shutdown();
        delete ContextManagerInstance;
    }
}
