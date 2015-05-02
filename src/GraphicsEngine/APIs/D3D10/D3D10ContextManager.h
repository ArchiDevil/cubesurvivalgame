#pragma once

#include <exception>
#include <vector>

#include "D3D10Context.h"
#include "D3D10Program.h"
#include "D3D10Mesh.h"
#include "D3D10ShaderGenerator.h"
#include "D3D10TextureManager.h"
#include "D3D10MeshManager.h"
#include "D3D10RenderTarget.h"
#include "D3D10ShaderManager.h"

#include "../../IContextManager.h"
#include "../../IVertexDeclaration.h"
#include "../../MiscTypes.h"
#include "../../MaterialManager.h"
#include "../../Sprite.h"
#include "../../Material.h"
#include "../../FontManager.h"

using MathLib::Vector2F;
using MathLib::vec3;
using MathLib::vec4;

namespace ShiftEngine
{
    class D3D10ContextManager : public IContextManager
    {
    public:
        D3D10ContextManager(HWND hwnd);
        ~D3D10ContextManager();

        bool                                Initialize(GraphicEngineSettings _Settings, PathSettings _Paths) override;
        std::wstring                        GetGPUDescription() override;
        void                                BeginScene() override;
        void                                EndScene() override;
        void                                ResetPipeline() override;
        ITexturePtr                         LoadTexture(const std::wstring & FileName) override;
        MaterialPtr                         LoadMaterial(const std::wstring & FileName, const std::wstring & mtlName) override;
        IProgramPtr                         LoadShader(const std::wstring & FileName) override;
        IMeshDataPtr                        LoadMesh(const std::wstring & FileName) override;
        D3D10ShaderManager *                GetShaderManager() override;
        D3D10ShaderGenerator *              GetShaderGenerator() override;
        D3D10TextureManager *               GetTextureManager() override;
        IMeshManager *                      GetMeshManager() override;
        FontManager*                        GetFontManager() override;
        void                                SetZState(bool enabled) override;
        void                                SetBlendingState(BlendingState bs) override;
        BlendingState                       GetBlendingState() const override;
        void                                SetRasterizerState(RasterizerState rs) override;
        RasterizerState                     GetRasterizerState() const override;
        GraphicEngineSettings               GetEngineSettings() const override;
        PathSettings                        GetPaths() const override;
        int                                 DrawMesh(IMeshDataPtr & mesh) override;
        IVertexDeclarationPtr               GetVertexDeclaration(const VertexSemantic & semantic) override;

    private:
        IVertexDeclarationPtr               CreateVDFromDescription(const VertexSemantic & semantic);

        HWND                                            windowHandle;
        PathSettings                                    enginePaths;
        GraphicEngineSettings                           engineSettings;

        std::map<VertexSemantic, IVertexDeclarationPtr> declarations;

        D3D10Context                                    graphicsContext;
        FontManager*                                    fontManager = nullptr;
        D3D10TextureManager *                           textureManager = nullptr;
        D3D10MeshManager *                              meshManager = nullptr;
        D3D10ShaderManager *                            shaderManager = nullptr;
        D3D10ShaderGenerator *                          shaderGenerator = nullptr;
        MaterialManager *                               materialManager = nullptr;

        IProgramPtr                                     currentProgram;
        RasterizerState                                 currentRasterizerState = RasterizerState::Normal;
        BlendingState                                   currentBlendingState = BlendingState::None;
        IVertexDeclaration *                            currentVertexDeclaration = nullptr;

        bool                                            zBufferState = true;
        bool                                            cullingEnabled = true;
    };

}   //end of ShiftEngine namespace
