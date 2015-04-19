#pragma once

#include "IVertexDeclaration.h"
#include "MaterialManager.h"
#include "Sprite.h"
#include "Material.h"
#include "FontManager.h"
#include "MiscTypes.h"
#include "IMeshManager.h"
#include "IShaderGenerator.h"

namespace ShiftEngine
{
    enum class BlendingState
    {
        None,
        AlphaEnabled,
        Additive
    };

    enum class RasterizerState
    {
        Wireframe,
        Normal,
        NoCulling
    };

    class IContextManager
    {
    public:
        virtual ~IContextManager() {}

        virtual bool                    Initialize(GraphicEngineSettings settings, PathSettings paths) = 0;
        virtual std::wstring            GetGPUDescription() = 0;
        virtual void                    BeginScene() = 0;
        virtual void                    EndScene() = 0;
        virtual void                    ResetPipeline() = 0;
        virtual ITexturePtr             LoadTexture(const std::wstring & FileName) = 0;
        virtual MaterialPtr             LoadMaterial(const std::wstring & FileName, const std::wstring & mtlName) = 0;
        virtual IProgramPtr             LoadShader(const std::wstring & FileName) = 0;
        virtual IMeshDataPtr            LoadMesh(const std::wstring & FileName) = 0;
        virtual IShaderManager *        GetShaderManager() = 0;
        virtual IShaderGenerator *      GetShaderGenerator() = 0;
        virtual ITextureManager *       GetTextureManager() = 0;
        virtual IMeshManager *          GetMeshManager() = 0;
        virtual FontManager*            GetFontManager() = 0;
        virtual void                    SetZState(bool enabled) = 0;
        virtual void                    SetBlendingState(BlendingState bs) = 0;
        virtual BlendingState           GetBlendingState() const = 0;
        virtual void                    SetRasterizerState(RasterizerState rs) = 0;
        virtual RasterizerState         GetRasterizerState() const = 0;
        virtual GraphicEngineSettings   GetEngineSettings() const = 0;
        virtual PathSettings            GetPaths() const = 0;
        virtual int                     DrawMesh(IMeshDataPtr & mesh) = 0;
        virtual IVertexDeclarationPtr   GetVertexDeclaration(const VertexSemantic & semantic) = 0;
    };
}
