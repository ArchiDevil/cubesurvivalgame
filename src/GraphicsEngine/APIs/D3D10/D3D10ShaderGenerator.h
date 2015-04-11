#pragma once

#include <vector>

#include "../../IShaderGenerator.h"
#include "../../VertexTypes.h"

namespace ShiftEngine
{
    class D3D10ShaderGenerator : public IShaderGenerator
    {
    public:
        std::string CreateShaderCode(const VertexSemantic & verticesInfo, const MaterialInfo & info) override;

    private:
        void CreateInput(std::ostringstream & stream, const VertexSemantic & verticesInfo);
        void CreateOutput(std::ostringstream & stream, const VertexSemantic & verticesInfo, const MaterialInfo & info);
        void CreateVS(std::ostringstream & stream, const VertexSemantic & verticesInfo, const MaterialInfo & info);
        void CreatePS(std::ostringstream & stream, const VertexSemantic & verticesInfo, const MaterialInfo & info);

        std::string GetTypeBySemantic(ElemType type, unsigned count);
        std::string GetNameBySemantic(const VertexInfo & info);
        std::string GetSemanticBySemantic(ElemSemantic sem);
        void CreateUniforms(std::ostringstream & stream, const MaterialInfo & info);
        void CreateSamplers(std::ostringstream & stream, const MaterialInfo & info);
    };
}
