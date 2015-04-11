#pragma once

#include <string>
#include <unordered_map>

#include "D3D10ShaderGenerator.h"
#include "D3D10Program.h"
#include "D3D10ShaderProgram.h"

#include "../../IShaderManager.h"
#include "../../MiscTypes.h"
#include "../../Utils.h"

namespace ShiftEngine
{
    class D3D10ShaderManager : public IShaderManager
    {
    public:
        D3D10ShaderManager(ID3D10Device * _pDevice);
        ~D3D10ShaderManager();

        virtual IProgramPtr CreateProgramFromFile(const std::wstring & fileName) override;
        virtual IProgramPtr CreateProgramFromSource(const std::string & source) override;
        virtual IProgramPtr CreateProgramFromMaterialFlags(const MaterialInfo & mtlInfo, const VertexSemantic & verticesInfo) override;

    private:
        D3D10ShaderPtr CompileVSFromSource(const std::string & source);
        D3D10ShaderPtr CompilePSFromSource(const std::string & source);

        ID3D10Device * pDevice = nullptr;
        std::unique_ptr<D3D10ShaderGenerator> shaderGenerator = nullptr;

        std::map<std::pair<MaterialInfo, VertexSemantic>, IProgramPtr> materialPrograms;
        std::unordered_map<std::wstring, IProgramPtr> filePrograms;
        std::unordered_map<std::string, IProgramPtr> sourcePrograms;

    };

} //end of namespace ShiftEngine
