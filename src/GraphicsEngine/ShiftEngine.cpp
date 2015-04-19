#include "ShiftEngine.h"

#include "APIs/D3D10/D3D10ContextManager.h"

namespace ShiftEngine
{
    IContextManager * ContextManagerInstance;
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

    IContextManager * GetContextManager()
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
        delete ContextManagerInstance;
    }
}
