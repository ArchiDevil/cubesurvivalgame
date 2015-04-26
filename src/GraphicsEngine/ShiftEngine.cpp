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

    bool InitOpenGLApi(HWND /*hwnd*/, GraphicEngineSettings /*settings*/, PathSettings /*paths*/)
    {
        return false;
    }

    bool InitEngine(const GraphicEngineSettings & settings, const PathSettings & paths, HWND hwnd)
    {
#if defined(D3D10_RENDER)
        return InitDX10Api(hwnd, settings, paths);
#elif defined(D3D11_RENDER)
        return InitDX11Api(hwnd, settings, paths);
#elif defined(OPENGL_RENDER)
        return InitOpenGLApi(hwnd, settings, paths);
#endif;
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
