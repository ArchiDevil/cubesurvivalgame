#pragma once

#include <vector>

#include "VertexTypes.h"
#include "RenderQueue.h"
#include "IShaderManager.h"
#include "IShaderGenerator.h"

#ifdef D3D10_RENDER
#include "D3D10ContextManager.h"
#elif OPENGL_RENDER
#include "OpenGLContextManager.h"
#endif

#include <Utilities/timer.h>

namespace ShiftEngine
{
    class Renderer
    {
        struct RendererState
        {
            RendererState()
                : DrawCalls(0)
                , PolygonsCount(0)
                , TextureBindings(0)
                , UniformsBindings(0)
                , MatricesBindings(0)
                , ShaderChanges(0)
                , currentProgram(nullptr)
                , shaderChanged(true)
                , materialChanged(false)
            {
            }

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

            unsigned int DrawCalls;
            unsigned int PolygonsCount;
            unsigned int ShaderChanges;
            unsigned int TextureBindings;
            unsigned int UniformsBindings;
            unsigned int MatricesBindings;

            bool shaderChanged;
            bool materialChanged;

            std::unordered_map<engineTextures, TexturePtr> texturesCache;

            IProgramPtr currentProgram;
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
