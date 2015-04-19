#include "ShiftEngine.h"

#ifdef D3D10_RENDER
#include "APIs/D3D10/D3D10ContextManager.h"
#elif D3D11_RENDER
#include "APIs/D3D11/D3D11ContextManager.h"
#endif

namespace ShiftEngine
{
    IContextManager * ContextManagerInstance;
    SceneGraph * SceneGraphInstance;
    Renderer * RendererInstance;

    bool InitDX10Api(HWND hwnd, GraphicEngineSettings settings, PathSettings paths)
    {
#ifdef D3D10_RENDER
        ContextManagerInstance = new D3D10ContextManager(hwnd);
        if (!ContextManagerInstance->Initialize(settings, paths))
            return false;
        RendererInstance = new Renderer(ContextManagerInstance->GetShaderManager(), ContextManagerInstance->GetShaderGenerator());
        SceneGraphInstance = new SceneGraph(SceneGraphType::SGT_QuadTree);
        return true;
#else
        return false;
#endif
    }

    bool InitDX11Api(HWND hwnd, GraphicEngineSettings settings, PathSettings paths)
    {
#ifdef D3D11_RENDER
        ContextManagerInstance = new D3D11ContextManager(hwnd);
        if (!ContextManagerInstance->Initialize(settings, paths))
            return false;
        RendererInstance = new Renderer(ContextManagerInstance->GetShaderManager(), ContextManagerInstance->GetShaderGenerator());
        SceneGraphInstance = new SceneGraph(SceneGraphType::SGT_QuadTree);
        return true;
#else
        return false;
#endif
    }

    bool InitGL33Api(HWND /*hwnd*/, GraphicEngineSettings /*settings*/, PathSettings /*paths*/)
    {
        return false;
    }

    bool InitEngine(API_TYPE apiType, GraphicEngineSettings settings, PathSettings paths, HWND hwnd)
    {
        switch (apiType)
        {
        case ShiftEngine::AT_DX10:  return InitDX10Api(hwnd, settings, paths);
        case ShiftEngine::AT_DX11:  return InitDX11Api(hwnd, settings, paths);
        case ShiftEngine::AT_OGL33: return InitGL33Api(hwnd, settings, paths);
        }
        return false;
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
