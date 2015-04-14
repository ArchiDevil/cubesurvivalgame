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

        void                                BeginScene();
        void                                EndScene();
        void                                ResetPipeline();

        ITexturePtr                         LoadTexture(const std::wstring & FileName);
        MaterialPtr                         LoadMaterial(const std::wstring & FileName, const std::wstring & mtlName);
        IProgramPtr                         LoadShader(const std::wstring & FileName);
        IMeshDataPtr                        LoadMesh(const std::wstring & FileName);

        D3D10ShaderManager *                GetShaderManager();
        D3D10ShaderGenerator *              GetShaderGenerator();
        D3D10TextureManager *               GetTextureManager();
        IMeshManager *                      GetMeshManager();
        FontManager*                        GetFontManager();

        void                                SetZState(bool enabled);
        void                                SetBlendingState(BlendingState bs);
        BlendingState                       GetBlendingState() const;
        void                                SetRasterizerState(RasterizerState rs);
        RasterizerState                     GetRasterizerState() const;

        ID3D10Device *                      GetDevicePointer();
        GraphicEngineSettings               GetEngineSettings() const;
        PathSettings                        GetPaths() const;
        int                                 DrawMesh(IMeshDataPtr & mesh);
        IVertexDeclarationPtr               GetVertexDeclaration(const VertexSemantic & semantic);

    private:
        IVertexDeclarationPtr               CreateVDFromDescription(const VertexSemantic & semantic);

        HWND                                                windowHandle;
        PathSettings                                        enginePaths;
        GraphicEngineSettings                               engineSettings;

        std::map<VertexSemantic, IVertexDeclarationPtr>     declarations;

        D3D10Context                                        graphicsContext;
        FontManager*                                        fontManager;
        D3D10TextureManager *                               textureManager;
        D3D10MeshManager *                                  meshManager;
        D3D10ShaderManager *                                shaderManager;
        D3D10ShaderGenerator *                              shaderGenerator;
        MaterialManager *                                   materialManager;

        IProgramPtr                                         currentProgram;
        RasterizerState                                     currentRasterizerState = RS_Normal;
        BlendingState                                       currentBlendingState = BS_None;
        IVertexDeclaration *                                currentVertexDeclaration;

        bool                                                zBufferState = true;
        bool                                                cullingEnabled = true;
    };

}   //end of ShiftEngine namespace
