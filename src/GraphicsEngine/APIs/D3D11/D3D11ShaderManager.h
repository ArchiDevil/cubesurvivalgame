#pragma once

#include <string>
#include <unordered_map>

#include "../../IShaderManager.h"
#include "D3D11ShaderGenerator.h"
#include "D3D11Program.h"
#include "D3D11ShaderProgram.h"
#include "../../MiscTypes.h"
#include "../../Utils.h"

namespace ShiftEngine
{
    class D3D11ShaderManager : public IShaderManager
    {
    public:
        D3D11ShaderManager(ID3D11Device * _pDevice);
        ~D3D11ShaderManager();

        virtual IProgramPtr CreateProgramFromFile(const std::wstring & fileName) override;
        virtual IProgramPtr CreateProgramFromSource(const std::string & source) override;
        virtual IProgramPtr CreateProgramFromMaterialFlags(const MaterialInfo & mtlInfo, const VertexSemantic & verticesInfo) override;

    private:
        D3D11ShaderPtr CompileVSFromSource(const std::string & source);
        D3D11ShaderPtr CompilePSFromSource(const std::string & source);

        ID3D11Device * pDevice = nullptr;
        std::unique_ptr<D3D11ShaderGenerator> shaderGenerator = nullptr;

        std::map<std::pair<MaterialInfo, VertexSemantic>, IProgramPtr> materialPrograms;
        std::unordered_map<std::wstring, IProgramPtr> filePrograms;
        std::unordered_map<std::string, IProgramPtr> sourcePrograms;

    };

} //end of namespace ShiftEngine
