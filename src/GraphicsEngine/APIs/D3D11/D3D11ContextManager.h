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

#include "../../IProgram.h"
#include "../../MiscTypes.h"
#include "../../MaterialManager.h"
#include "../../Sprite.h"
#include "../../TextLib/cText.h"
#include "../../Material.h"
#include "../../FontManager.h"

using MathLib::Vector2F;
using MathLib::vec3;
using MathLib::vec4;

namespace ShiftEngine
{
    enum BlendingState
    {
        BS_None,
        BS_AlphaEnabled,
        BS_Additive,
    };

    enum RasterizerState
    {
        RS_Wireframe,
        RS_Normal,
        RS_NoCulling,
    };

    class D3D11ContextManager
    {
        friend class Renderer;
    public:
        D3D11ContextManager(HWND hwnd);
        ~D3D11ContextManager();

        virtual bool                        Initialize(GraphicEngineSettings _Settings, PathSettings _Paths);

        std::wstring                        GetGPUDescription();

        void                                Shutdown();
        void                                BeginScene();
        void                                EndScene();
        void                                ResetPipeline();

        ITexturePtr                         LoadTexture(const std::wstring & FileName);
        MaterialPtr                         LoadMaterial(const std::wstring & FileName, const std::wstring & mtlName);
        IProgramPtr                         LoadShader(const std::wstring & FileName);
        D3D11MeshDataPtr                    LoadMesh(const std::wstring & FileName);

        D3D11ShaderManager *                GetShaderManager();
        D3D11ShaderGenerator *              GetShaderGenerator();
        D3D11TextureManager *               GetTextureManager();
        FontManager*                        GetFontManager();

        void                                SetZState(bool enabled);
        void                                SetBlendingState(BlendingState bs);
        BlendingState                       GetBlendingState() const;
        void                                SetRasterizerState(RasterizerState rs);
        RasterizerState                     GetRasterizerState() const;

        ID3D11Device *                      GetDevicePointer();
        GraphicEngineSettings               GetEngineSettings() const;
        PathSettings                        GetPaths() const;
        int                                 DrawMesh(D3D11MeshDataPtr & mesh);
        D3D11VDPtr                          GetVertexDeclaration(const VertexSemantic & semantic);
        D3DMATRIX                           GetOrthoMatrix() const;

    private:
        void                                RegisterVertexSemantic(const VertexSemantic & semantic);
        ShiftEngine::D3D11VertexDeclaration CreateVDFromDescription(const VertexSemantic & semantic);

        HWND                                windowHandle;
        PathSettings                        enginePaths;
        GraphicEngineSettings               engineSettings;

        std::map<VertexSemantic, D3D11VDPtr> declarations;

        D3D11Context                        graphicsContext;
        FontManager*                        fontManager;
        D3D11TextureManager *               textureManager;
        D3D11MeshManager *                  meshManager;
        D3D11ShaderManager *                shaderManager;
        D3D11ShaderGenerator *              shaderGenerator;
        MaterialManager *                   materialManager;

        IProgramPtr                         currentProgram;
        RasterizerState                     currentRasterizerState = RS_Normal;
        BlendingState                       currentBlendingState = BS_None;
        D3D11VertexDeclaration *            currentVertexDeclaration;

        bool                                zBufferState = true;
        bool                                cullingEnabled = true;
    };

}   //end of ShiftEngine namespace
