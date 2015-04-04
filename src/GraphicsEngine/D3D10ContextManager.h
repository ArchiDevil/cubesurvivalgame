#pragma once

#include <exception>
#include <vector>

#include "D3D10Context.h"
#include "D3D10Program.h"
#include "cMesh.h"
#include "MiscTypes.h"
#include "D3D10ShaderManager.h"
#include "D3D10ShaderGenerator.h"
#include "D3D10TextureManager.h"
#include "D3D10MeshManager.h"
#include "MaterialManager.h"
#include "Sprite.h"
#include "D3D10RenderTarget.h"
#include "TextLib/cText.h"
#include "Material.h"
#include "D3D10VertexDeclaration.h"
#include "FontManager.h"

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

    class D3D10ContextManager
    {
        friend class Renderer;
    public:
        D3D10ContextManager(HWND hwnd);
        ~D3D10ContextManager();

        virtual bool                        Initialize(GraphicEngineSettings _Settings, PathSettings _Paths);

        std::wstring                        GetGPUDescription();

        void                                Shutdown();
        void                                BeginScene();
        void                                EndScene();
        void                                ResetPipeline();

        TexturePtr                          LoadTexture(const std::wstring & FileName);
        MaterialPtr                         LoadMaterial(const std::wstring & FileName, const std::wstring & mtlName);
        IProgramPtr                         LoadShader(const std::wstring & FileName);
        MeshDataPtr                         LoadMesh(const std::wstring & FileName);

        D3D10ShaderManager *                GetShaderManager();
        D3D10ShaderGenerator *              GetShaderGenerator();
        D3D10TextureManager *               GetTextureManager();
        FontManager*                        GetFontManager();

        void                                SetZState(bool enabled);
        void                                SetBlendingState(BlendingState bs);
        BlendingState                       GetBlendingState() const;
        void                                SetRasterizerState(RasterizerState rs);
        RasterizerState                     GetRasterizerState() const;

        ID3D10Device *                      GetDevicePointer();
        GraphicEngineSettings               GetEngineSettings() const;
        PathSettings                        GetPaths() const;
        int                                 DrawMesh(MeshDataPtr & mesh);
        D3D10VDPtr                          GetVertexDeclaration(const VertexSemantic & semantic);
        D3DMATRIX                           GetOrthoMatrix() const;

    private:
        void                                RegisterVertexSemantic(const VertexSemantic & semantic);
        ShiftEngine::D3D10VertexDeclaration CreateVDFromDescription(const VertexSemantic & semantic);

        HWND                                windowHandle;
        PathSettings                        enginePaths;
        GraphicEngineSettings               engineSettings;

        std::map<VertexSemantic, D3D10VDPtr> declarations;

        D3D10Context                        graphicsContext;
        FontManager*                        fontManager;
        D3D10TextureManager *               textureManager;
        D3D10MeshManager *                  meshManager;
        D3D10ShaderManager *                shaderManager;
        D3D10ShaderGenerator *              shaderGenerator;
        MaterialManager *                   materialManager;

        IProgramPtr                         currentProgram;
        RasterizerState                     currentRasterizerState = RS_Normal;
        BlendingState                       currentBlendingState = BS_None;
        D3D10VertexDeclaration *            currentVertexDeclaration;

        bool                                zBufferState = true;
        bool                                cullingEnabled = true;
    };

}	//end of ShiftEngine namespace
