#pragma once

#include <exception>
#include <vector>

#include "D3D11Context.h"
#include "D3D11Mesh.h"
#include "D3D11ShaderManager.h"
#include "D3D11ShaderGenerator.h"
#include "D3D11TextureManager.h"
#include "D3D11MeshManager.h"
#include "D3D11RenderTarget.h"
#include "D3D11VertexDeclaration.h"

#include "../../IContextManager.h"
#include "../../IProgram.h"
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
    class D3D11ContextManager : public IContextManager
    {
        friend class Renderer;
    public:
        D3D11ContextManager(HWND hwnd);
        ~D3D11ContextManager();

        bool                                Initialize(GraphicEngineSettings _Settings, PathSettings _Paths) override;
        std::wstring                        GetGPUDescription() override;
        void                                BeginScene() override;
        void                                EndScene() override;
        void                                ResetPipeline() override;
        ITexturePtr                         LoadTexture(const std::wstring & FileName) override;
        MaterialPtr                         LoadMaterial(const std::wstring & FileName, const std::wstring & mtlName) override;
        IProgramPtr                         LoadShader(const std::wstring & FileName) override;
        IMeshDataPtr                        LoadMesh(const std::wstring & FileName) override;
        IShaderManager *                    GetShaderManager() override;
        IShaderGenerator *                  GetShaderGenerator() override;
        ITextureManager *                   GetTextureManager() override;
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

        HWND                                windowHandle;
        PathSettings                        enginePaths;
        GraphicEngineSettings               engineSettings;

        std::map<VertexSemantic, IVertexDeclarationPtr> declarations;

        D3D11Context                        graphicsContext;
        FontManager*                        fontManager;
        D3D11TextureManager *               textureManager;
        D3D11MeshManager *                  meshManager;
        D3D11ShaderManager *                shaderManager;
        D3D11ShaderGenerator *              shaderGenerator;
        MaterialManager *                   materialManager;

        IProgramPtr                         currentProgram;
        RasterizerState                     currentRasterizerState = RasterizerState::Normal;
        BlendingState                       currentBlendingState = BlendingState::None;
        IVertexDeclaration *                currentVertexDeclaration;

        bool                                zBufferState = true;
        bool                                cullingEnabled = true;
    };

}   //end of ShiftEngine namespace
