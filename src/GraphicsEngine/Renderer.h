#pragma once

#include <vector>
#include <unordered_map>

#include "VertexTypes.h"
#include "RenderQueue.h"
#include "IShaderManager.h"
#include "IShaderGenerator.h"

#ifdef D3D10_RENDER
#   include "APIs/D3D10/D3D10ContextManager.h"
#elif D3D11_RENDER
#   include "APIs/D3D11/D3D11ContextManager.h"
#elif OPENGL_RENDER
#   include "APIs/OGL33/OpenGLContextManager.h"
#endif

#include <Utilities/timer.h>

namespace ShiftEngine
{
    class Renderer
    {
        struct RendererState
        {
            void Reset()
            {
                DrawCalls = 0;
                PolygonsCount = 0;
                ShaderChanges = 0;
                TextureBindings = 0;
                UniformsBindings = 0;
                MatricesBindings = 0;
                shaderChanged = true;
                materialChanged = true;
                currentProgram = nullptr;

                texturesCache.clear();
            }

            unsigned int DrawCalls = 0;
            unsigned int PolygonsCount = 0;
            unsigned int ShaderChanges = 0;
            unsigned int TextureBindings = 0;
            unsigned int UniformsBindings = 0;
            unsigned int MatricesBindings = 0;

            bool shaderChanged = true;
            bool materialChanged = true; // was false in default constructor

            std::unordered_map<engineTextures, ITexturePtr> texturesCache;

            IProgramPtr currentProgram = nullptr;
        };

    public:
        Renderer(IShaderManager * _pShaderManager, IShaderGenerator * _pShaderGenerator);

        void DrawAll(RenderQueue & rq, double dt);

        unsigned int GetDrawCalls() const;
        unsigned int GetDrawnPolygonsCount() const;
        unsigned int GetTextureBindings() const;
        unsigned int GetUniformsBindings() const;
        unsigned int GetMatricesBindings() const;

        double GetElapsedTime() const;
        int GetFPS() const;
        double GetMSPF() const;
        unsigned int GetShaderChanges() const;

    private:
        void PreProcess();
        void Process(RenderQueue & rq);
        void PostProcess();

        void bindEngineUniforms(MeshNode * currentNode, const RenderQueue & list);
        void bindCustomUniforms(MeshNode * currentNode, const RenderQueue & list);

        void drawSky(RenderQueue &rq);
        void bindLights(const LightsVector & lv, unsigned int startIndex, unsigned int count, Material * matPtr);

        RendererState currentState;

        cTimer FPSTimer;
        double elapsedTime;
        int	FPSCounter;
        int	FPS;
        double millisecondsPerFrame;

        IShaderManager * pShaderManager;
        IShaderGenerator * pShaderGenerator;
    };
}
